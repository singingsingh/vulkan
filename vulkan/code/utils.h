#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace vulkan {
	void printExtensionsAndLayersInfo();
	bool checkValidationLayerSupport();
	const std::vector<VkLayerProperties> getValidationLayers();
	std::vector<const char*> getRequiredExtension();
	void printRequiredExtension();
} // namespace vulkan