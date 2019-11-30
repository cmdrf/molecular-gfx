/*	GlfwVulkanWindow.cpp

MIT License

Copyright (c) 2019 Fabian Herb

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "GlfwVulkanWindow.h"
#include "Logging.h"
#include <stdexcept>

namespace molecular
{
namespace gfx
{

#define DEMO_TEXTURE_COUNT 1
#define APP_SHORT_NAME "mol"

#define GET_INSTANCE_PROC_ADDR(inst, entrypoint)                               \
	{                                                                          \
		demo->fp##entrypoint = (PFN_vk##entrypoint)vkGetInstanceProcAddr(inst, "vk" #entrypoint); \
		if (demo->fp##entrypoint == NULL) {                                    \
			throw std::runtime_error("vkGetInstanceProcAddr failed to find vk" #entrypoint);                         \
		}                                                                      \
	}

#define GET_DEVICE_PROC_ADDR(dev, entrypoint)                                  \
	{                                                                          \
		demo->fp##entrypoint = (PFN_vk##entrypoint)vkGetDeviceProcAddr(dev, "vk" #entrypoint); \
		if (demo->fp##entrypoint == NULL) {                                    \
			throw std::runtime_error("vkGetDeviceProcAddr failed to find vk" #entrypoint);                           \
		}                                                                      \
	}

VKAPI_ATTR VkBool32 VKAPI_CALL
BreakCallback(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType,
			  uint64_t srcObject, size_t location, int32_t msgCode,
			  const char *pLayerPrefix, const char *pMsg,
			  void *pUserData)
{
	LOG(FATAL) << pMsg;

	return false;
}

struct texture_object {
	VkSampler sampler;

	VkImage image;
	VkImageLayout imageLayout;

	VkDeviceMemory mem;
	VkImageView view;
	int32_t tex_width, tex_height;
};

typedef struct {
	VkImage image;
	VkCommandBuffer cmd;
	VkImageView view;
} SwapchainBuffers;

struct demo
{
	VkSurfaceKHR surface = nullptr;
	bool use_staging_buffer = false;

	VkInstance inst = nullptr;
	VkPhysicalDevice gpu = nullptr;
	VkDevice device = nullptr;
	VkQueue queue = nullptr;
	VkPhysicalDeviceProperties gpu_props;
	VkPhysicalDeviceFeatures gpu_features;
	std::vector<VkQueueFamilyProperties> queue_props;
	uint32_t graphics_queue_node_index = 0;

	uint32_t enabled_extension_count = 0;
	uint32_t enabled_layer_count = 0;
	const char *extension_names[64] = {0};
	const char *enabled_layers[64] = {0};

	VkFormat format = VK_FORMAT_UNDEFINED;
	VkColorSpaceKHR color_space;

	PFN_vkGetPhysicalDeviceSurfaceSupportKHR fpGetPhysicalDeviceSurfaceSupportKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR fpGetPhysicalDeviceSurfaceCapabilitiesKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR fpGetPhysicalDeviceSurfaceFormatsKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR fpGetPhysicalDeviceSurfacePresentModesKHR = nullptr;
	PFN_vkCreateSwapchainKHR fpCreateSwapchainKHR = nullptr;
	PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR = nullptr;
	PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR = nullptr;
	PFN_vkAcquireNextImageKHR fpAcquireNextImageKHR = nullptr;
	PFN_vkQueuePresentKHR fpQueuePresentKHR = nullptr;
	uint32_t swapchainImageCount = 0;
	VkSwapchainKHR swapchain = nullptr;
	std::vector<SwapchainBuffers> buffers;

	VkCommandPool cmd_pool = nullptr;

	struct {
		VkFormat format;

		VkImage image = nullptr;
		VkDeviceMemory mem = nullptr;
		VkImageView view = nullptr;
	} depth;

	struct texture_object textures[DEMO_TEXTURE_COUNT];

	struct {
		VkBuffer buf = nullptr;
		VkDeviceMemory mem = nullptr;

		VkPipelineVertexInputStateCreateInfo vi;
		VkVertexInputBindingDescription vi_bindings[1];
		VkVertexInputAttributeDescription vi_attrs[2];
	} vertices;

	VkCommandBuffer setup_cmd = VK_NULL_HANDLE; // Command Buffer for initialization commands
	VkCommandBuffer draw_cmd = nullptr;  // Command Buffer for drawing commands
	VkPipelineLayout pipeline_layout = nullptr;
	VkDescriptorSetLayout desc_layout = nullptr;
	VkPipelineCache pipelineCache = nullptr;
	VkRenderPass render_pass = nullptr;
	VkPipeline pipeline = nullptr;

	VkShaderModule vert_shader_module = nullptr;
	VkShaderModule frag_shader_module = nullptr;

	VkDescriptorPool desc_pool = nullptr;
	VkDescriptorSet desc_set = nullptr;

	std::vector<VkFramebuffer> framebuffers;

	VkPhysicalDeviceMemoryProperties memory_properties;

	int32_t curFrame = 0;
	int32_t frameCount = 0;
	bool validate = false;
	bool use_break = false;
	PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallback = nullptr;
	PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallback = nullptr;
	VkDebugReportCallbackEXT msg_callback = nullptr;
	PFN_vkDebugReportMessageEXT DebugReportMessage = nullptr;

	float depthStencil = 0;
	float depthIncrement = 0;

	uint32_t current_buffer = 0;
	uint32_t queue_count = 0;
};

static int validation_error = 0;

VKAPI_ATTR VkBool32 VKAPI_CALL
dbgFunc(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType,
	uint64_t srcObject, size_t location, int32_t msgCode,
	const char *pLayerPrefix, const char *pMsg, void *pUserData)
{
	char *message = (char *)malloc(strlen(pMsg) + 100);

	assert(message);

	validation_error = 1;

	if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
		sprintf(message, "ERROR: [%s] Code %d : %s", pLayerPrefix, msgCode, pMsg);
	else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
		sprintf(message, "WARNING: [%s] Code %d : %s", pLayerPrefix, msgCode, pMsg);
	else
		return false;

	printf("%s\n", message);
	fflush(stdout);
	free(message);

	/*
	* false indicates that layer should not bail-out of an
	* API call that had validation failures. This may mean that the
	* app dies inside the driver due to invalid parameter(s).
	* That's what would happen without validation layers, so we'll
	* keep that behavior here.
	*/
	return false;
}

/*
 * Return true if all layer names specified in check_names
 * can be found in given layer properties.
 */
static bool demo_check_layers(uint32_t check_count, const char **check_names,
								  uint32_t layer_count,
								  VkLayerProperties *layers)
{
	uint32_t i, j;
	for (i = 0; i < check_count; i++)
	{
		bool found = false;
		for (j = 0; j < layer_count; j++)
		{
			if (!strcmp(check_names[i], layers[j].layerName))
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			LOG(INFO)<< "Cannot find layer: " << check_names[i];
			return false;
		}
	}
	return true;
}

static void demo_init_vk(struct demo *demo)
{
	uint32_t required_extension_count = 0;
	uint32_t instance_extension_count = 0;
	uint32_t instance_layer_count = 0;
	uint32_t validation_layer_count = 0;
	const char **instance_validation_layers = NULL;
	demo->enabled_extension_count = 0;
	demo->enabled_layer_count = 0;

	std::array<const char*, 1> instance_validation_layers_alt1 = {{
		"VK_LAYER_LUNARG_standard_validation"
	}};

	std::array<const char*, 7> instance_validation_layers_alt2 = {{
		"VK_LAYER_GOOGLE_threading",
		"VK_LAYER_LUNARG_parameter_validation",
		"VK_LAYER_LUNARG_object_tracker",
		"VK_LAYER_LUNARG_image",
		"VK_LAYER_LUNARG_core_validation",
		"VK_LAYER_LUNARG_swapchain",
		"VK_LAYER_GOOGLE_unique_objects"
	}};

	/* Look for validation layers */
	bool validation_found = false;
	if (demo->validate)
	{
		VkResult err = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);
		assert(!err);

		instance_validation_layers = instance_validation_layers_alt1.data();
		if (instance_layer_count > 0)
		{
			std::vector<VkLayerProperties> instance_layers(instance_layer_count);
			err = vkEnumerateInstanceLayerProperties(&instance_layer_count, instance_layers.data());
			assert(!err);

			validation_found = demo_check_layers(
					instance_validation_layers_alt1.size(),
					instance_validation_layers, instance_layer_count,
					instance_layers.data());
			if (validation_found)
			{
				demo->enabled_layer_count = instance_validation_layers_alt1.size();
				demo->enabled_layers[0] = "VK_LAYER_LUNARG_standard_validation";
				validation_layer_count = 1;
			}
			else
			{
				// use alternative set of validation layers
				instance_validation_layers = instance_validation_layers_alt2.data();
				demo->enabled_layer_count = instance_validation_layers_alt2.size();
				validation_found = demo_check_layers(
					instance_validation_layers_alt2.size(),
					instance_validation_layers, instance_layer_count,
					instance_layers.data());
				validation_layer_count = instance_validation_layers_alt2.size();
				for (unsigned int i = 0; i < validation_layer_count; i++)
					demo->enabled_layers[i] = instance_validation_layers[i];
			}
		}

		if (!validation_found)
			throw std::runtime_error("vkEnumerateInstanceLayerProperties failed to find required validation layer.");
	}

	/* Look for instance extensions */
	const char** required_extensions = glfwGetRequiredInstanceExtensions(&required_extension_count);
	if (!required_extensions)
		throw std::runtime_error("glfwGetRequiredInstanceExtensions failed to find the platform surface extensions.");

	for (unsigned int i = 0; i < required_extension_count; i++)
	{
		demo->extension_names[demo->enabled_extension_count++] = required_extensions[i];
		assert(demo->enabled_extension_count < 64);
	}

	VkResult err = vkEnumerateInstanceExtensionProperties(nullptr, &instance_extension_count, nullptr);
	assert(!err);

	if (instance_extension_count > 0)
	{
		std::vector<VkExtensionProperties> instance_extensions(instance_extension_count);
		err = vkEnumerateInstanceExtensionProperties(NULL, &instance_extension_count, instance_extensions.data());
		assert(!err);
		for (unsigned int i = 0; i < instance_extension_count; i++)
		{
			if (!strcmp(VK_EXT_DEBUG_REPORT_EXTENSION_NAME, instance_extensions[i].extensionName))
			{
				if (demo->validate)
					demo->extension_names[demo->enabled_extension_count++] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
			}
			assert(demo->enabled_extension_count < 64);
		}
	}

	const vk::ApplicationInfo app = {
		APP_SHORT_NAME,
		0,
		APP_SHORT_NAME,
		0,
		VK_API_VERSION_1_0,
	};
	vk::InstanceCreateInfo inst_info {
		{},
		&app,
		demo->enabled_layer_count,
		instance_validation_layers,
		demo->enabled_extension_count,
		demo->extension_names
	};

	demo->inst = vk::createInstance(inst_info);

	/* Make initial call to query gpu_count, then second call for gpu info*/
	uint32_t gpu_count = 0;
	err = vkEnumeratePhysicalDevices(demo->inst, &gpu_count, nullptr);
	assert(!err && gpu_count > 0);

	if (gpu_count > 0)
	{
		std::vector<VkPhysicalDevice> physical_devices(gpu_count);
		err = vkEnumeratePhysicalDevices(demo->inst, &gpu_count, physical_devices.data());
		assert(!err);
		/* For tri demo we just grab the first physical device */
		demo->gpu = physical_devices[0];
	}
	else
		throw std::runtime_error("vkEnumeratePhysicalDevices Failure");

	/* Look for device extensions */
	uint32_t device_extension_count = 0;
	bool swapchainExtFound = false;
	demo->enabled_extension_count = 0;

	err = vkEnumerateDeviceExtensionProperties(demo->gpu, NULL, &device_extension_count, NULL);
	assert(!err);

	if (device_extension_count > 0)
	{
		std::vector<VkExtensionProperties> device_extensions(device_extension_count);
		err = vkEnumerateDeviceExtensionProperties(demo->gpu, NULL, &device_extension_count, device_extensions.data());
		assert(!err);

		for (unsigned int i = 0; i < device_extension_count; i++)
		{
			if (!strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME, device_extensions[i].extensionName))
			{
				swapchainExtFound = true;
				demo->extension_names[demo->enabled_extension_count++] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
			}
			assert(demo->enabled_extension_count < 64);
		}
	}

	if (!swapchainExtFound)
		throw std::runtime_error("vkCreateInstance Failure");

	if (demo->validate)
	{
		demo->CreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(demo->inst, "vkCreateDebugReportCallbackEXT");
		demo->DestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(demo->inst, "vkDestroyDebugReportCallbackEXT");
		if (!demo->CreateDebugReportCallback)
			throw std::runtime_error("GetProcAddr: Unable to find vkCreateDebugReportCallbackEXT");
		if (!demo->DestroyDebugReportCallback)
			throw std::runtime_error("GetProcAddr: Unable to find vkDestroyDebugReportCallbackEXT");
		demo->DebugReportMessage = (PFN_vkDebugReportMessageEXT)vkGetInstanceProcAddr(demo->inst, "vkDebugReportMessageEXT");
		if (!demo->DebugReportMessage)
			throw std::runtime_error("GetProcAddr: Unable to find vkDebugReportMessageEXT");

		VkDebugReportCallbackCreateInfoEXT dbgCreateInfo;
		dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		dbgCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
		dbgCreateInfo.pfnCallback = demo->use_break ? BreakCallback : dbgFunc;
		dbgCreateInfo.pUserData = demo;
		dbgCreateInfo.pNext = NULL;
		err = demo->CreateDebugReportCallback(demo->inst, &dbgCreateInfo, NULL, &demo->msg_callback);
		switch (err)
		{
		case VK_SUCCESS:
			break;

		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw std::runtime_error("CreateDebugReportCallback: out of host memory");

		default:
			throw std::runtime_error("CreateDebugReportCallback: unknown failure");
		}
	}

	// Having these GIPA queries of device extension entry points both
	// BEFORE and AFTER vkCreateDevice is a good test for the loader
	GET_INSTANCE_PROC_ADDR(demo->inst, GetPhysicalDeviceSurfaceCapabilitiesKHR);
	GET_INSTANCE_PROC_ADDR(demo->inst, GetPhysicalDeviceSurfaceFormatsKHR);
	GET_INSTANCE_PROC_ADDR(demo->inst, GetPhysicalDeviceSurfacePresentModesKHR);
	GET_INSTANCE_PROC_ADDR(demo->inst, GetPhysicalDeviceSurfaceSupportKHR);

	vkGetPhysicalDeviceProperties(demo->gpu, &demo->gpu_props);

	// Query with NULL data to get count
	vkGetPhysicalDeviceQueueFamilyProperties(demo->gpu, &demo->queue_count, NULL);

	demo->queue_props.resize(demo->queue_count);
	vkGetPhysicalDeviceQueueFamilyProperties(demo->gpu, &demo->queue_count, demo->queue_props.data());
	assert(demo->queue_count >= 1);

	vkGetPhysicalDeviceFeatures(demo->gpu, &demo->gpu_features);

	// Graphics queue and MemMgr queue can be separate.
	// TODO: Add support for separate queues, including synchronization,
	//       and appropriate tracking for QueueSubmit
}

void GlfwVulkanWindow::demo_create_window(const char* title)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	mWindow = glfwCreateWindow(mWidth, mHeight, title, nullptr, nullptr);
	if(!mWindow)
	{
		// It didn't work, so try to give a useful error:
		throw std::runtime_error("Cannot create a window in which to draw!");
	}
}

static void demo_init_device(struct demo *demo)
{
	float queue_priorities[1] = {0.0};
	const VkDeviceQueueCreateInfo queue = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.pNext = NULL,
		.queueFamilyIndex = demo->graphics_queue_node_index,
		.queueCount = 1,
		.pQueuePriorities = queue_priorities};


	VkPhysicalDeviceFeatures features;
	memset(&features, 0, sizeof(features));
	if (demo->gpu_features.shaderClipDistance)
		features.shaderClipDistance = VK_TRUE;

	VkDeviceCreateInfo device = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = nullptr,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &queue,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = nullptr,
		.enabledExtensionCount = demo->enabled_extension_count,
		.ppEnabledExtensionNames = (const char *const *)demo->extension_names,
		.pEnabledFeatures = &features,
	};

	VkResult err = vkCreateDevice(demo->gpu, &device, nullptr, &demo->device);
	assert(!err);

	GET_DEVICE_PROC_ADDR(demo->device, CreateSwapchainKHR);
	GET_DEVICE_PROC_ADDR(demo->device, DestroySwapchainKHR);
	GET_DEVICE_PROC_ADDR(demo->device, GetSwapchainImagesKHR);
	GET_DEVICE_PROC_ADDR(demo->device, AcquireNextImageKHR);
	GET_DEVICE_PROC_ADDR(demo->device, QueuePresentKHR);
}

void GlfwVulkanWindow::demo_init_vk_swapchain(struct demo *demo)
{
	// Create a WSI surface for the window:
	glfwCreateWindowSurface(demo->inst, mWindow, NULL, &demo->surface);

	// Iterate over each queue to learn whether it supports presenting:
	std::vector<VkBool32> supportsPresent(demo->queue_count);
	for (unsigned int i = 0; i < demo->queue_count; i++)
		demo->fpGetPhysicalDeviceSurfaceSupportKHR(demo->gpu, i, demo->surface, &supportsPresent[i]);

	// Search for a graphics and a present queue in the array of queue
	// families, try to find one that supports both
	uint32_t graphicsQueueNodeIndex = UINT32_MAX;
	uint32_t presentQueueNodeIndex = UINT32_MAX;
	for (uint32_t i = 0; i < demo->queue_count; i++)
	{
		if ((demo->queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
		{
			if (graphicsQueueNodeIndex == UINT32_MAX)
				graphicsQueueNodeIndex = i;

			if (supportsPresent[i] == VK_TRUE)
			{
				graphicsQueueNodeIndex = i;
				presentQueueNodeIndex = i;
				break;
			}
		}
	}
	if (presentQueueNodeIndex == UINT32_MAX)
	{
		// If didn't find a queue that supports both graphics and present, then
		// find a separate present queue.
		for (uint32_t i = 0; i < demo->queue_count; ++i)
		{
			if (supportsPresent[i] == VK_TRUE)
			{
				presentQueueNodeIndex = i;
				break;
			}
		}
	}

	// Generate error if could not find both a graphics and a present queue
	if (graphicsQueueNodeIndex == UINT32_MAX || presentQueueNodeIndex == UINT32_MAX)
		throw std::runtime_error("Could not find a graphics and a present queue");

	// TODO: Add support for separate queues, including presentation,
	//       synchronization, and appropriate tracking for QueueSubmit.
	// NOTE: While it is possible for an application to use a separate graphics
	//       and a present queues, this demo program assumes it is only using
	//       one:
	if (graphicsQueueNodeIndex != presentQueueNodeIndex)
		throw std::runtime_error("Could not find a common graphics and a present queue");

	demo->graphics_queue_node_index = graphicsQueueNodeIndex;

	demo_init_device(demo);

	vkGetDeviceQueue(demo->device, demo->graphics_queue_node_index, 0, &demo->queue);

	// Get the list of VkFormat's that are supported:
	uint32_t formatCount;
	VkResult err = demo->fpGetPhysicalDeviceSurfaceFormatsKHR(demo->gpu, demo->surface, &formatCount, NULL);
	assert(!err);
	std::vector<VkSurfaceFormatKHR> surfFormats(formatCount);
	err = demo->fpGetPhysicalDeviceSurfaceFormatsKHR(demo->gpu, demo->surface, &formatCount, surfFormats.data());
	assert(!err);
	// If the format list includes just one entry of VK_FORMAT_UNDEFINED,
	// the surface has no preferred format.  Otherwise, at least one
	// supported format will be returned.
	if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED)
		demo->format = VK_FORMAT_B8G8R8A8_UNORM;
	else
	{
		assert(formatCount >= 1);
		demo->format = surfFormats[0].format;
	}
	demo->color_space = surfFormats[0].colorSpace;

	demo->curFrame = 0;

	// Get Memory information and properties
	vkGetPhysicalDeviceMemoryProperties(demo->gpu, &demo->memory_properties);
}

void GlfwVulkanWindow::demo_prepare_buffers(struct demo *demo)
{
	VkSwapchainKHR oldSwapchain = demo->swapchain;

	// Check the surface capabilities and formats
	VkSurfaceCapabilitiesKHR surfCapabilities;
	VkResult err = demo->fpGetPhysicalDeviceSurfaceCapabilitiesKHR(demo->gpu, demo->surface, &surfCapabilities);
	assert(!err);

	uint32_t presentModeCount = 0;
	err = demo->fpGetPhysicalDeviceSurfacePresentModesKHR(demo->gpu, demo->surface, &presentModeCount, NULL);
	assert(!err);
	std::vector<VkPresentModeKHR> presentModes(presentModeCount);
	err = demo->fpGetPhysicalDeviceSurfacePresentModesKHR(demo->gpu, demo->surface, &presentModeCount, presentModes.data());
	assert(!err);

	VkExtent2D swapchainExtent;
	// width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
	if (surfCapabilities.currentExtent.width == 0xFFFFFFFF)
	{
		// If the surface size is undefined, the size is set to the size
		// of the images requested, which must fit within the minimum and
		// maximum values.
		swapchainExtent.width = mWidth;
		swapchainExtent.height = mHeight;

		if (swapchainExtent.width < surfCapabilities.minImageExtent.width)
			swapchainExtent.width = surfCapabilities.minImageExtent.width;
		else if (swapchainExtent.width > surfCapabilities.maxImageExtent.width)
			swapchainExtent.width = surfCapabilities.maxImageExtent.width;

		if (swapchainExtent.height < surfCapabilities.minImageExtent.height)
			swapchainExtent.height = surfCapabilities.minImageExtent.height;
		else if (swapchainExtent.height > surfCapabilities.maxImageExtent.height)
			swapchainExtent.height = surfCapabilities.maxImageExtent.height;

	}
	else
	{
		// If the surface size is defined, the swap chain size must match
		swapchainExtent = surfCapabilities.currentExtent;
		mWidth = surfCapabilities.currentExtent.width;
		mHeight = surfCapabilities.currentExtent.height;
	}

	VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

	// Determine the number of VkImage's to use in the swap chain.
	// Application desires to only acquire 1 image at a time (which is
	// "surfCapabilities.minImageCount").
	uint32_t desiredNumOfSwapchainImages = surfCapabilities.minImageCount;
	// If maxImageCount is 0, we can ask for as many images as we want;
	// otherwise we're limited to maxImageCount
	if(surfCapabilities.maxImageCount > 0 && desiredNumOfSwapchainImages > surfCapabilities.maxImageCount)
	{
		// Application must settle for fewer images than desired:
		desiredNumOfSwapchainImages = surfCapabilities.maxImageCount;
	}

	VkSurfaceTransformFlagBitsKHR preTransform;
	if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
		preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	else
		preTransform = surfCapabilities.currentTransform;

	const VkSwapchainCreateInfoKHR swapchain = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.pNext = NULL,
		.surface = demo->surface,
		.minImageCount = desiredNumOfSwapchainImages,
		.imageFormat = demo->format,
		.imageColorSpace = demo->color_space,
		.imageExtent =
			{
			 .width = swapchainExtent.width, .height = swapchainExtent.height,
			},
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.preTransform = preTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.imageArrayLayers = 1,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 0,
		.pQueueFamilyIndices = NULL,
		.presentMode = swapchainPresentMode,
		.oldSwapchain = oldSwapchain,
		.clipped = true,
	};
	uint32_t i;

	err = demo->fpCreateSwapchainKHR(demo->device, &swapchain, NULL,
									 &demo->swapchain);
	assert(!err);

	// If we just re-created an existing swapchain, we should destroy the old
	// swapchain at this point.
	// Note: destroying the swapchain also cleans up all its associated
	// presentable images once the platform is done with them.
	if (oldSwapchain != VK_NULL_HANDLE) {
		demo->fpDestroySwapchainKHR(demo->device, oldSwapchain, NULL);
	}

	err = demo->fpGetSwapchainImagesKHR(demo->device, demo->swapchain,
										&demo->swapchainImageCount, NULL);
	assert(!err);

	std::vector<VkImage> swapchainImages(demo->swapchainImageCount);
	err = demo->fpGetSwapchainImagesKHR(demo->device, demo->swapchain, &demo->swapchainImageCount, swapchainImages.data());
	assert(!err);

	demo->buffers.resize(demo->swapchainImageCount);

	for (i = 0; i < demo->swapchainImageCount; i++)
	{
		VkImageViewCreateInfo color_attachment_view = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = NULL,
			.format = demo->format,
			.components =
				{
				 .r = VK_COMPONENT_SWIZZLE_R,
				 .g = VK_COMPONENT_SWIZZLE_G,
				 .b = VK_COMPONENT_SWIZZLE_B,
				 .a = VK_COMPONENT_SWIZZLE_A,
				},
			.subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
								 .baseMipLevel = 0,
								 .levelCount = 1,
								 .baseArrayLayer = 0,
								 .layerCount = 1},
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.flags = 0,
		};

		demo->buffers[i].image = swapchainImages[i];

		color_attachment_view.image = demo->buffers[i].image;

		err = vkCreateImageView(demo->device, &color_attachment_view, NULL, &demo->buffers[i].view);
		assert(!err);
	}

	demo->current_buffer = 0;
}

static bool memory_type_from_properties(struct demo *demo, uint32_t typeBits,
										VkFlags requirements_mask,
										uint32_t *typeIndex)
{
	uint32_t i;
	// Search memtypes to find first index with those properties
	for (i = 0; i < VK_MAX_MEMORY_TYPES; i++) {
		if ((typeBits & 1) == 1) {
			// Type is available, does it match user properties?
			if ((demo->memory_properties.memoryTypes[i].propertyFlags &
				 requirements_mask) == requirements_mask) {
				*typeIndex = i;
				return true;
			}
		}
		typeBits >>= 1;
	}
	// No memory types matched, return failure
	return false;
}

static void demo_set_image_layout(struct demo *demo, VkImage image,
								  VkImageAspectFlags aspectMask,
								  VkImageLayout old_image_layout,
								  VkImageLayout new_image_layout,
								  VkAccessFlags srcAccessMask)
{
	if (demo->setup_cmd == VK_NULL_HANDLE)
	{
		const VkCommandBufferAllocateInfo cmd = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = NULL,
			.commandPool = demo->cmd_pool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1,
		};

		VkResult err = vkAllocateCommandBuffers(demo->device, &cmd, &demo->setup_cmd);
		assert(!err);

		VkCommandBufferBeginInfo cmd_buf_info = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = NULL,
			.flags = 0,
			.pInheritanceInfo = NULL,
		};
		err = vkBeginCommandBuffer(demo->setup_cmd, &cmd_buf_info);
		assert(!err);
	}

	VkImageMemoryBarrier image_memory_barrier = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.pNext = NULL,
		.srcAccessMask = srcAccessMask,
		.dstAccessMask = 0,
		.oldLayout = old_image_layout,
		.newLayout = new_image_layout,
		.image = image,
		.subresourceRange = {aspectMask, 0, 1, 0, 1}
	};

	if (new_image_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		/* Make sure anything that was copying from this image has completed */
		image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	}

	if (new_image_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
		image_memory_barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	if (new_image_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		image_memory_barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	if (new_image_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		/* Make sure any Copy or CPU writes to image are flushed */
		image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
	}

	VkImageMemoryBarrier *pmemory_barrier = &image_memory_barrier;

	VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

	vkCmdPipelineBarrier(demo->setup_cmd, srcStageMask, dstStageMask, 0, 0, NULL, 0, NULL, 1, pmemory_barrier);
}

void GlfwVulkanWindow::demo_prepare_depth(struct demo *demo)
{
	const VkFormat depth_format = VK_FORMAT_D16_UNORM;
	const VkImageCreateInfo image = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.pNext = NULL,
		.imageType = VK_IMAGE_TYPE_2D,
		.format = depth_format,
		.extent = {mWidth, mHeight, 1},
		.mipLevels = 1,
		.arrayLayers = 1,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.tiling = VK_IMAGE_TILING_OPTIMAL,
		.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		.flags = 0,
	};
	VkMemoryAllocateInfo mem_alloc = {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.pNext = NULL,
		.allocationSize = 0,
		.memoryTypeIndex = 0,
	};
	VkImageViewCreateInfo view = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.pNext = NULL,
		.image = VK_NULL_HANDLE,
		.format = depth_format,
		.subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
							 .baseMipLevel = 0,
							 .levelCount = 1,
							 .baseArrayLayer = 0,
							 .layerCount = 1},
		.flags = 0,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
	};

	demo->depth.format = depth_format;

	/* create image */
	VkResult err = vkCreateImage(demo->device, &image, NULL, &demo->depth.image);
	assert(!err);

	/* get memory requirements for this object */
	VkMemoryRequirements mem_reqs;
	vkGetImageMemoryRequirements(demo->device, demo->depth.image, &mem_reqs);

	/* select memory size and type */
	mem_alloc.allocationSize = mem_reqs.size;
	bool pass = memory_type_from_properties(demo, mem_reqs.memoryTypeBits,
									   0, /* No requirements */
									   &mem_alloc.memoryTypeIndex);
	assert(pass);

	/* allocate memory */
	err = vkAllocateMemory(demo->device, &mem_alloc, NULL, &demo->depth.mem);
	assert(!err);

	/* bind memory */
	err = vkBindImageMemory(demo->device, demo->depth.image, demo->depth.mem, 0);
	assert(!err);

	demo_set_image_layout(demo, demo->depth.image, VK_IMAGE_ASPECT_DEPTH_BIT,
						  VK_IMAGE_LAYOUT_UNDEFINED,
						  VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
						  0);

	/* create image view */
	view.image = demo->depth.image;
	err = vkCreateImageView(demo->device, &view, NULL, &demo->depth.view);
	assert(!err);
}

static void demo_prepare_render_pass(struct demo *demo)
{
	const VkAttachmentDescription attachments[2] = {
			[0] =
				{
				 .format = demo->format,
				 .samples = VK_SAMPLE_COUNT_1_BIT,
				 .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				 .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				 .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				 .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				 .initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				 .finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				},
			[1] =
				{
				 .format = demo->depth.format,
				 .samples = VK_SAMPLE_COUNT_1_BIT,
				 .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				 .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				 .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				 .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				 .initialLayout =
					 VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				 .finalLayout =
					 VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				},
	};
	const VkAttachmentReference color_reference = {
		.attachment = 0, .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
	};
	const VkAttachmentReference depth_reference = {
		.attachment = 1,
		.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
	};
	const VkSubpassDescription subpass = {
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.flags = 0,
		.inputAttachmentCount = 0,
		.pInputAttachments = NULL,
		.colorAttachmentCount = 1,
		.pColorAttachments = &color_reference,
		.pResolveAttachments = NULL,
		.pDepthStencilAttachment = &depth_reference,
		.preserveAttachmentCount = 0,
		.pPreserveAttachments = NULL,
	};
	const VkRenderPassCreateInfo rp_info = {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.pNext = NULL,
		.attachmentCount = 2,
		.pAttachments = attachments,
		.subpassCount = 1,
		.pSubpasses = &subpass,
		.dependencyCount = 0,
		.pDependencies = NULL,
	};
	VkResult err = vkCreateRenderPass(demo->device, &rp_info, NULL, &demo->render_pass);
	assert(!err);
}

void GlfwVulkanWindow::demo_prepare_framebuffers(struct demo *demo)
{
	VkImageView attachments[2];
	attachments[1] = demo->depth.view;

	const VkFramebufferCreateInfo fb_info = {
		.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		.pNext = nullptr,
		.renderPass = demo->render_pass,
		.attachmentCount = 2,
		.pAttachments = attachments,
		.width = mWidth,
		.height = mHeight,
		.layers = 1,
	};

	uint32_t i;

	demo->framebuffers.resize(demo->swapchainImageCount);

	for (i = 0; i < demo->swapchainImageCount; i++)
	{
		attachments[0] = demo->buffers[i].view;
		VkResult err = vkCreateFramebuffer(demo->device, &fb_info, nullptr, &demo->framebuffers[i]);
		assert(!err);
	}
}

GlfwVulkanWindow::GlfwVulkanWindow(const char* title)
{
	glfwInit();
	if(!glfwVulkanSupported())
		throw std::runtime_error("Vulkan not supported");

	auto monitor = glfwGetPrimaryMonitor();
	auto videoMode = glfwGetVideoMode(monitor);
	mWidth = videoMode->width;
	mHeight = videoMode->height;

	struct demo demo;
	demo.frameCount = INT32_MAX;

	demo.use_staging_buffer = false;
	demo.use_break = true;
	demo.validate = true;

	demo_init_vk(&demo);

	demo.depthStencil = 1.0;
	demo.depthIncrement = -0.01f;

	demo_create_window(title);
	demo_init_vk_swapchain(&demo);

	const VkCommandPoolCreateInfo cmd_pool_info = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.pNext = NULL,
		.queueFamilyIndex = demo.graphics_queue_node_index,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
	};
	VkResult err = vkCreateCommandPool(demo.device, &cmd_pool_info, NULL, &demo.cmd_pool);
	assert(!err);

	const VkCommandBufferAllocateInfo cmd = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.pNext = NULL,
		.commandPool = demo.cmd_pool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};
	err = vkAllocateCommandBuffers(demo.device, &cmd, &demo.draw_cmd);
	assert(!err);

	demo_prepare_buffers(&demo);
	demo_prepare_depth(&demo);
//	demo_prepare_textures(demo);
//	demo_prepare_vertices(demo);
//	demo_prepare_descriptor_layout(demo);
	demo_prepare_render_pass(&demo);
//	demo_prepare_pipeline(demo);

//	demo_prepare_descriptor_pool(demo);
//	demo_prepare_descriptor_set(demo);
	demo_prepare_framebuffers(&demo);
}

GlfwVulkanWindow::~GlfwVulkanWindow()
{
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

}
}
