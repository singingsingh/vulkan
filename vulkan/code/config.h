#pragma once

#include <stdint.h>

#include <vector>
#include <optional>

namespace vulkan {
const uint16_t WIDTH = 800;
const uint16_t HEIGHT = 600;

const std::vector<const char*> validation_layers = { "VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
};

} // namespace vulkan