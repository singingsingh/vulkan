#include <iostream>
#include <algorithm>
#include <cstring>
#include <cassert>
#include <fstream>

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

	bool isDeviceSuitable(const VkPhysicalDevice device, VkSurfaceKHR surface) {
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

		QueueFamilyIndices indices = findQueueFamilies(device, surface);
		if (indices.graphicsFamily.has_value() == false) {
			return false;
		}

		// check for device extennsion support
		uint32_t extension_count = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

		std::vector<VkExtensionProperties> extension{ extension_count };
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, extension.data());

		for (const char* required_extension : deviceExtensions) {
			bool found = false;
			for (const VkExtensionProperties& available_device_extension : extension) {
				if (strcmp(required_extension, available_device_extension.extensionName) == 0) {
					found = true;
					break;
				}
			}
			if (found == false) {
				return false;
			}
		}

		SwapChainSupportDetails swap_chain_details = querySwapChainSupport(device, surface);
		if (swap_chain_details.formats.size() < 1 || swap_chain_details.presentModes.size() < 1) {
			return false;
		}


		return true;
	}

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
		QueueFamilyIndices indices;

		
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

			if (presentSupport) {
				indices.presentFamily = i;
			}

			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
				break;
			}
			i++;
		}

		return indices;
	}

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		SwapChainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t surface_format_count = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &surface_format_count, nullptr);
		details.formats.resize(surface_format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &surface_format_count, details.formats.data());

		uint32_t present_modes = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_modes, nullptr);
		details.presentModes.resize(present_modes);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_modes, details.presentModes.data());

		return details;
	}

	void printPhysicalDeviceInfo(const VkPhysicalDevice& device) {
		VkPhysicalDeviceProperties device_properties{};
		vkGetPhysicalDeviceProperties(device, &device_properties);
		assert(device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
		std::cout << "Selected GPU : " << device_properties.deviceName << "\n";
	}

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window) {
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	std::vector<char> readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device) {
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}

		return shaderModule;
	}
} // namespace vulkan