#include "config.h"

namespace vkInit
{
    // Creates and returns a Vulkan Instance.
    // This is the root Vulkan object that connects the application
    // to the Vulkan runtime and GPU driver.
    vk::Instance make_instance(bool debug, const char *applicationName)
    {
        if (debug)
        {
            std::cout << "Making an Instance From (instance.h)\n";
        }

        // Stores the highest Vulkan API version supported by the system.
        uint32_t version{0};

        // Query the Vulkan loader for the maximum supported API version.
        vkEnumerateInstanceVersion(&version);

        // Print the Vulkan version supported by the system.
        if (debug)
        {
            std::cout << "System can support:\n";
            std::cout << "Variant : " << VK_API_VERSION_VARIANT(version) << '\n';
            std::cout << "Major   : " << VK_API_VERSION_MAJOR(version) << '\n';
            std::cout << "Minor   : " << VK_API_VERSION_MINOR(version) << '\n';
            std::cout << "Patch   : " << VK_API_VERSION_PATCH(version) << '\n';
        }

        // Specify the Vulkan API version that this application wants to use.
        // Even if the system supports a newer version, the application can
        // request an older version for compatibility.
        version = VK_MAKE_API_VERSION(0, 1, 0, 0);

        // Describe the application to the Vulkan runtime.
        // This information is mainly used by validation layers,
        // debugging tools (RenderDoc), and Vulkan drivers.
        vk::ApplicationInfo appInfo(
            applicationName, // Application name
            version,         // Application version
            "Vulkan Engine", // Engine name
            version,         // Engine version
            version          // Requested Vulkan API version
        );

        // Number of Vulkan instance extensions required by GLFW.
        uint32_t glfwExtensionCount = 0;

        // Pointer that will receive the array of extension names.
        const char **glfwExtensions = nullptr;

        // Ask GLFW which Vulkan instance extensions are required
        // for the current operating system (Windows/Linux/macOS).
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        // Copy the extension names into a C++ vector for easier handling.
        std::vector<const char *> extensions(
            glfwExtensions,
            glfwExtensions + glfwExtensionCount);

        // Print all required Vulkan instance extensions.
        if (debug)
        {
            std::cout << "Required GLFW Extensions:\n";

            for (const char *extension : extensions)
            {
                std::cout << "\t" << extension << '\n';
            }
        }

        // Fill the Vulkan InstanceCreateInfo structure.
        // This structure tells Vulkan everything needed to create
        // the VkInstance.
        vk::InstanceCreateInfo createInfo(
            vk::InstanceCreateFlags(),                   // No special creation flags
            &appInfo,                                   // Application information
            0,                                          // Validation layer count (deprecated)
            nullptr,                                    // Validation layer names (deprecated)
            static_cast<uint32_t>(extensions.size()),   // Number of required extensions
            extensions.data()                           // Pointer to extension name array
        );

        try
        {
            // Create the Vulkan Instance.
            // If successful, this becomes the root object for all
            // subsequent Vulkan operations.
            return vk::createInstance(createInfo, nullptr);
        }
        catch (vk::SystemError err)
        {
            // Instance creation failed.
            // Print an error message in debug builds.
            if (debug)
            {
                std::cout << "Failed to create Vulkan Instance!\n";
                std::cout << err.what() << '\n';
            }

            return nullptr;
        }
    }
}