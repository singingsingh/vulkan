#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "engine.h"

int main() {
    vulkan::Engine app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}