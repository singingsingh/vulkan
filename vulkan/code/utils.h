#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vulkan {
	void printExtensionsAndLayersInfo();
	bool checkValidationLayerSupport();
	const std::vector<VkLayerProperties> getValidationLayers();
} // namespace vulkan