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
		createInstance();
		printExtensionsAndLayersInfo();
		if (enableValidationLayers && checkValidationLayerSupport() == false) {
			throw std::runtime_error("required validation layer not found.");
		}
	}

	void Engine::mainLoop() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}

	void Engine::cleanup() {
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
		instance_create_info.enabledLayerCount = 0;
		instance_create_info.pApplicationInfo = &application_info;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		instance_create_info.enabledExtensionCount = glfwExtensionCount;
		instance_create_info.ppEnabledExtensionNames = glfwExtensions;

		if (vkCreateInstance(&instance_create_info, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("Cannot create instance");
		}
	}

	void Engine::run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
} // namespace vulkan