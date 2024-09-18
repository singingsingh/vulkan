#include <iostream>
#include <vector>
#include <cstring>

#include "utils.h"

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
		std::cout << "\n";
	}

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

	void printPhysicalDeviceInfo(std::vector<VkPhysicalDevice> devices) {
		std::cout << "Found " << devices.size() << " physical devices";
		for (const VkPhysicalDevice& device : devices) {
			VkPhysicalDeviceProperties device_properties{};
			vkGetPhysicalDeviceProperties(device, &device_properties);
			std::cout << "\n\t" << device_properties.deviceName << " " << device_properties.deviceType;
		}
	}

	bool isDeviceSuitable(const VkPhysicalDevice device) {
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);	
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			return false;
		}
		if (deviceFeatures.geometryShader == VK_FALSE) {
			return false;
		}

		QueueFamilyIndices indices = findQueueFamilies(device);
		if (indices.graphicsFamily.has_value() == false) {
			return false;
		}

		return true;
	}

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
		QueueFamilyIndices indices;
		
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
				break;
			}
			i++;
		}

		return indices;
	}

	void printPhysicalDeviceInfo(const VkPhysicalDevice& device) {
		VkPhysicalDeviceProperties device_properties{};
		vkGetPhysicalDeviceProperties(device, &device_properties);
		std::cout << "Selected GPU : " << device_properties.deviceName << " " << device_properties.deviceType;
	}
} // namespace vulkan