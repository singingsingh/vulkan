#pragma once

#include <vector>
#include <array>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdint>

#include <glm/glm.hpp>

namespace vulkan {
	class Engine {
	public:
		void run() {
			initWindow();
			initVulkan();
			mainLoop();
			cleanUp();
		}

	private:
		void initWindow();
		void initVulkan();
		void mainLoop();
		void cleanUp();
		void createInstance();
		void setupDebugMessenger();
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void pickPhysicalDevice();
		bool isDeviceSuitable(VkPhysicalDevice device);

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData) {

			std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

			return VK_FALSE;
		}

	private:
		struct QueueFamilyIndices {
			uint32_t graphicsFamily;
		};

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		GLFWwindow* window;
		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
	};
} // namespace vulkan