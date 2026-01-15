#include "utils.h"

#include <vector>
#include <iostream>
namespace vulkan
{
	void printInstanceExtensions() {
		uint32_t extension_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
		std::vector<VkExtensionProperties> extensions(extension_count);
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

		for (uint32_t i = 0; i < extension_count; i++) {
			std::cout << "Extension Name " << extensions[i].extensionName << "\n";
		}		
	}
}