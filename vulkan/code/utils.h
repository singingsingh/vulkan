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
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
} // namespace vulkan