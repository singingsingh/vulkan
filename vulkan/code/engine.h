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
			createInstance();
		}

	private:
		void initWindow();
		void initVulkan();
		void mainLoop();
		void cleanUp();
		void createInstance();

	private:
		GLFWwindow* window;
		VkInstance instance;
	};
} // namespace vulkan