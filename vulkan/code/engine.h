#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vulkan {
class Engine {
public:
    void run();

private:
    void initVulkan();
    void mainLoop();
    void cleanup();

    GLFWwindow* window = nullptr;
};
} // namespace vulkan