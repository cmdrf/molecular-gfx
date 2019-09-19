/*	VulkanInfrastructure.cpp
	Copyright 2018 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "VulkanInfrastructure.h"
#include "Logging.h"

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
