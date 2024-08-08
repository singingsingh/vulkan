#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vulkan {
	class Engine {
	public:
		void run();

	private:
		void initWindow();
		void initVulkan();
		void mainLoop();
		void cleanup();
		void createInstance();

	private:
		GLFWwindow* window = nullptr;
		VkInstance instance;
	};
} // namespace vulkan