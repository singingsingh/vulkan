#include <iostream>
#include <vector>
#include <cstring>

#include "utils.h"
#include "config.h"

namespace vulkan {
	void printExtensionsAndLayersInfo() {
		uint32_t extension_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
		std::cout << "extension_count : " << extension_count << "\n";

		std::vector<VkExtensionProperties> extension_properties(extension_count);
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extension_properties.data());

		for (const VkExtensionProperties& property : extension_properties) {
			std::cout << property.extensionName << " " << property.specVersion << "\n";
		}

		std::cout << "\n";

		uint32_t layer_count = 0;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
		std::cout << "layer_count : " << layer_count << "\n";

		std::vector<VkLayerProperties> layer_properties(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, layer_properties.data());
		for (const VkLayerProperties& layer : layer_properties) {
			std::cout << layer.description << " " << layer.layerName << "\n";
		}

		std::cout << "\n";
	}

	bool checkValidationLayerSupport() {
		const std::vector<VkLayerProperties> available_layers = getValidationLayers();
		for (const char* required_layer : validation_layers) {
			bool layer_found = false;
			for (const VkLayerProperties& available_layer : available_layers) {
				if (strcmp(available_layer.layerName, required_layer) == 0) {
					layer_found = true;
				}
			}
			if (layer_found == false) {
				std::cout << required_layer << " validation layer not found\n";
				return false;
			}
		}

		return true;
	}

	const std::vector<VkLayerProperties> getValidationLayers() {
		uint32_t layer_count = 0;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
		std::vector<VkLayerProperties> layer_properties(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, layer_properties.data());
		return layer_properties;
	}

	std::vector<const char*> getRequiredExtension() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = nullptr;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}
	void printRequiredExtension() {
		std::vector<const char*> required_extensions = getRequiredExtension();
		std::cout << "Required extension : ";
		for (const char* extension : required_extensions) {
			std::cout << "\n\t" << extension;
		}
	}
} // namespace vulkan