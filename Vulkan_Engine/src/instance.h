// File name : instance.h

#include "config.h"

namespace vkInit
{
    // Checking the Supported - Extension and Layers
    bool supported(std::vector<const char *> &extensions, std::vector<const char *> &layers, bool debug)
    {
        // Extension Having
        std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();
        if (debug)
        {
            std::cout << std::endl ; 
            std::cout << " ----------extension checking----------------------------------------- \n ";
            std::cout << "Device can Supported Following Extensions:\n";
            for (vk::ExtensionProperties supportExtension : supportedExtensions)
            {
                std::cout << supportExtension.extensionName << std::endl;
            }
        }
        // Supporting Extension
        bool found;
        for (const char *extension : extensions)
        {
            found = false;
            for (vk::ExtensionProperties supportedExtension : supportedExtensions)
            {
                if (strcmp(extension, supportedExtension.extensionName) == 0)
                {
                    found = true;
                    if (debug)
                        std::cout << "Extension - " << extension << " - Supported " << std::endl;
                }
            }
            if (!found)
            {
                if (debug)
                {
                    std::cout << "Extension - " << extension << " - Not Supported " << std::endl;
                }
            }
        }

        // Layers Having
        std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();
        if (debug)
        {
            std::cout << " --------extension checking end------------------------------------------ \n ";
            std::cout << std::endl ; 
            std::cout << " ----------layers checking ----------------------------------------- \n ";
            std::cout << "Supported Following Extensions:\n";
            for (vk::LayerProperties supportedLayer : supportedLayers)
            {
                std::cout << supportedLayer.layerName << std::endl;
            }
            std::cout << " ----------layers checking end----------------------------------------- \n ";
        }
        for (const char* layer : layers)
        {
            found = false;
            for (vk::LayerProperties supportedLayer : supportedLayers)
            {
                if (strcmp(layer, supportedLayer.layerName) == 0)
                {
                    found = true;
                    if (debug)
                        std::cout << "Layers - " << layer << " - Supported " << std::endl;
                }
            }
            if (!found)
            {
                if (debug)
                {
                    std::cout << "Layers - " << layer << " - Not Supported " << std::endl;
                }
            }
        }

        return true;
    }

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
            if(debug)
            {
                extensions.push_back("VK_EXT_debug_utils") ; 
            }

        // Print all required Vulkan instance extensions.
        if (debug)
        {
            std::cout << "Required GLFW Extensions:\n";

            for (const char *extension : extensions)
            {
                std::cout << "\t" << extension << '\n';
            }
        }

        // Handling the LAYERS
        std::vector<const char *> layers;
        if (debug)
        {
            layers.push_back("VK_LAYER_KHRONOS_validation");
        }

        if (!supported(extensions, layers, debug))
        {
            return nullptr;
        }

        // Fill the Vulkan InstanceCreateInfo structure.
        // This structure tells Vulkan everything needed to create
        // the VkInstance.
        vk::InstanceCreateInfo createInfo(
            vk::InstanceCreateFlags(),                           // No special creation flags
            &appInfo,                                            // Application information                                                  // Validation layer count (deprecated)
            static_cast<uint32_t>(layers.size()), layers.data(), // Validation layer names (deprecated)
            static_cast<uint32_t>(extensions.size()),            // Number of required extensions
            extensions.data()                                    // Pointer to extension name array
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