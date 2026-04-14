#include "utils.h"

#include <vector>
#include <iostream>
namespace vulkan
{
	void printInstanceExtensions() {
		uint32_t extension_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
		std::vector<VkExtensionProperties> extensions(extension_count);
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

		for (uint32_t i = 0; i < extension_count; i++) {
			std::cout << "Extension Name " << extensions[i].extensionName << "\n";
		}		
	}

	void printInstanceValidationLayers() {
		uint32_t layer_count = 0;
		vkEnumerateInstanceLayerProperties(&layer_count,nullptr);

		std::vector<VkLayerProperties> layers(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, layers.data());

		for (uint32_t i = 0; i < layer_count; i++) {
			std::cout << "Layer name : " << layers[i].layerName << "\n";
		}
	}

	bool checkValidationLayerSupport(const std::vector<const char*> required_layers) {
		uint32_t layer_count = 0;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

		std::vector<VkLayerProperties> layers(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, layers.data());

		bool found = false;

		for (const auto& required_layer : required_layers) {
			for (const auto& available_layer : layers) {
				if (strcmp(required_layer, available_layer.layerName) == 0) {
					found = true;
				}
			}

			if (found == false) {
				return false;
			}
		}
		return true;
	}

	std::vector<const char*> getRequiredExtensions(const bool enableValidationLayers) {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}
}