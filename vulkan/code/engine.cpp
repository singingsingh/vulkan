#include <stdexcept>

#include <cassert>

#include "engine.h"
#include "config.h"
#include "utils.h"

namespace vulkan {
	void Engine::initWindow() {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Sample", nullptr, nullptr);
	}

	void Engine::initVulkan() {
		printExtensionsAndLayersInfo();
		printRequiredExtension();
		if (enableValidationLayers && checkValidationLayerSupport() == false) {
			throw std::runtime_error("required validation layer not found.");
		}
		createInstance();
		setupDebugMessenger();
	}

	void Engine::mainLoop() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}

	void Engine::cleanup() {
		if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}
		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void Engine::createInstance() {
		VkApplicationInfo application_info{};
		application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		application_info.apiVersion = VK_API_VERSION_1_0;
		application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		application_info.pApplicationName = "vulkan_vikram";
		application_info.pNext = nullptr;

		VkInstanceCreateInfo instance_create_info{};
		instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_create_info.pNext = nullptr;
		
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (enableValidationLayers) {
			instance_create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
			instance_create_info.ppEnabledLayerNames = validation_layers.data();

			populateDebugMessengerCreateInfo(debugCreateInfo);
			instance_create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else {
			instance_create_info.enabledLayerCount = 0;

			instance_create_info.pNext = nullptr;
		}

		instance_create_info.pApplicationInfo = &application_info;

		auto extensions = getRequiredExtension();
		instance_create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		instance_create_info.ppEnabledExtensionNames = extensions.data();


		if (vkCreateInstance(&instance_create_info, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("Cannot create instance");
		}
	}

	VkBool32 Engine::debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		std::cout << "validation layer : " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	void Engine::setupDebugMessenger() {
		if (!enableValidationLayers) {
			return;
		}

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		populateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug messenger!");
		}
	}

	void Engine::run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

	void Engine::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}
} // namespace vulkan