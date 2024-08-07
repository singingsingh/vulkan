#include "engine.h"
#include "config.h"

namespace vulkan {
	void Engine::initVulkan() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Sample", nullptr, nullptr);
    }
	void Engine::mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

	void Engine::cleanup() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Engine::run() {
        initVulkan();
        mainLoop();
        cleanup();
    }
} // namespace vulkan