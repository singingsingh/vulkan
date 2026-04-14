#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace vulkan {
	void printInstanceExtensions();
	void printInstanceValidationLayers();
	bool checkValidationLayerSupport(const std::vector<const char*> required_layers);
	std::vector<const char*> getRequiredExtensions(const bool enableValidationLayers);
}
