/*	VulkanInfrastructure.cpp

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

#include "VulkanInfrastructure.h"
#include <molecular/util/Logging.h>

namespace molecular
{

VulkanInfrastructure::VulkanInfrastructure()
{

}

void VulkanInfrastructure::Init()
{
	vk::ApplicationInfo applicationInfo;
	applicationInfo.pApplicationName = "MolecularEngineRunner";
	applicationInfo.pEngineName = "MolecularEngine";
	applicationInfo.apiVersion = VK_API_VERSION_1_0;

	vk::InstanceCreateInfo instanceCreateInfo;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;

	mInstance = vk::createInstance(instanceCreateInfo);

	auto physicalDevices = mInstance.enumeratePhysicalDevices();
	for(auto& physicalDevice: physicalDevices)
	{
		auto properties = physicalDevice.getProperties();
		LOG(INFO) << "Vulkan physical device name: " << properties.deviceName;
	}

	auto& physicalDevice = physicalDevices.front();
	auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
	LOG(INFO) << "Vulkan queue family count: " << queueFamilyProperties.size();
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo;
	deviceQueueCreateInfo.queueCount = 1;
	bool found = false;
	for(size_t i = 0; i < queueFamilyProperties.size(); i++)
	{
		if(queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics)
		{
			deviceQueueCreateInfo.queueFamilyIndex  = i;
			found = true;
			break;
		}
	}
	if(!found)
		throw std::runtime_error("No queue with VK_QUEUE_GRAPHICS_BIT found");

	vk::DeviceCreateInfo deviceCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
	auto device = physicalDevice.createDevice(deviceCreateInfo);

	// Create a command buffer:
	vk::CommandPoolCreateInfo commandPoolCreateInfo(vk::CommandPoolCreateFlags(), deviceQueueCreateInfo.queueFamilyIndex);
	auto commandPool = device.createCommandPool(commandPoolCreateInfo);

	vk::CommandBufferAllocateInfo commandBufferAllocateInfo;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.commandBufferCount = 1;

	// Create a swapchain:
	// TODO

}

void VulkanInfrastructure::Exit()
{
	mInstance.destroy();
}

}
