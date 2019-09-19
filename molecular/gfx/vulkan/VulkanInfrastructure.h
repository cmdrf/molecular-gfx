/*	VulkanInfrastructure.h
	Copyright 2018 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef VULKANINFRASTRUCTURE_H
#define VULKANINFRASTRUCTURE_H

#include <vulkan/vulkan.hpp>

class VulkanInfrastructure
{
public:
	VulkanInfrastructure();

	void Init();
	void Exit();

private:
	vk::Instance mInstance;
};

#endif // VULKANINFRASTRUCTURE_H
