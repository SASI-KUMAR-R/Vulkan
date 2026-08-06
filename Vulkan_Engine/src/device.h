#pragma once
#include "config.h"

namespace vkInit
{
    struct QueueFamilyIndices
    {
        std::optional<u_int32_t> graphicsFamily ; 
        std::optional<u_int32_t> presentFamily ; 

        bool isComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value() ; 
        }
    } ; 

    QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device,bool debugMode)
    {
        QueueFamilyIndices indices ; 

        std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties() ; 
        if(debugMode)
        {
            std::cout << "--------------QueueFamilies----------" << std::endl ; 
            std::cout << "System Can Support - " << queueFamilies.size() << " queue Families " << std::endl ; 
        }

        int i = 0 ; 
        for(vk::QueueFamilyProperties queueFamily : queueFamilies)
        {
            if(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
            {
                indices.graphicsFamily = i ; 
                indices.presentFamily = i ; 

                if(debugMode)
                {
                    std::cout<< "QueueFamily " << i << " is sutiable for graphics \n" ;
                }
            }
            if(indices.isComplete())
            {
                break; 
            }
            i++ ; 
        }

        return indices ; 
    }
    // -------------------------------------------------------------------------
    // FUNCTION: log_device_properties
    // PURPOSE:  Prints out human-readable information about a specific GPU.
    //           This helps in debugging to see what hardware Vulkan detects.
    // -------------------------------------------------------------------------
    void log_device_properties(const vk::PhysicalDevice &device)
    {
        // Query the physical device for its core properties (Name, Type, Limits, etc.)
        vk::PhysicalDeviceProperties properties = device.getProperties();

        // Print the actual name of the hardware (e.g., "NVIDIA GeForce RTX 3060")
        std::cout << "Device Name - " << properties.deviceName << std::endl;
        
        std::cout << "Device Type : ";
        // Determine what kind of hardware this is using a switch statement.
        // This is useful because we usually prefer Discrete GPUs for gaming/rendering.
        switch (properties.deviceType)
        {
        case (vk::PhysicalDeviceType::eCpu):
            std::cout << "CPU\n"; // Software rendering (very slow)
            break;
        case (vk::PhysicalDeviceType::eDiscreteGpu):
            std::cout << "Discrete GPU\n"; // Dedicated graphics card (e.g., PCIe card)
            break;
        case (vk::PhysicalDeviceType::eIntegratedGpu): 
            std::cout << "Integrated GPU\n"; // Built into the CPU (e.g., Intel UHD)
            break;
        case (vk::PhysicalDeviceType::eVirtualGpu): 
            std::cout << "Virtual GPU\n"; // Running in a virtual machine
            break;
        case (vk::PhysicalDeviceType::eOther): 
            std::cout << "Other\n";
            break;
        default:
            std::cout << "Unknown\n";
            break;
        }
    }

    // -------------------------------------------------------------------------
    // FUNCTION: checkDeviceExtensionSupport
    // PURPOSE:  Checks if a specific GPU supports the extra features (extensions)
    //           our program needs to run.
    // -------------------------------------------------------------------------
    bool checkDeviceExtensionSupport(const vk::PhysicalDevice& device, const std::vector<const char*>& requestedExtensions, bool debugMode)
    {
        // STEP 1: Put all the extensions we *need* into a std::set.
        // A std::set makes it very easy to remove items and check if it's empty.
        std::set<std::string> requiredExtensions(requestedExtensions.begin(), requestedExtensions.end()); 
        
        if(debugMode)
        {
            std::cout << "Device can support: \n"; 
        }

        // STEP 2: Ask the GPU for every single extension it supports, and loop through them.
        for(vk::ExtensionProperties& extension : device.enumerateDeviceExtensionProperties())
        {
            if(debugMode)
            {
                std::cout << "\t\"" << extension.extensionName << "\"\n"; 
            }
            
            // STEP 3: If the GPU supports an extension, try to remove it from our "required" set.
            // If the extension isn't in our set, erase() just safely does nothing.
            requiredExtensions.erase(extension.extensionName); 
        }

        // STEP 4: If our set is empty, it means we found and crossed off every single
        // extension we asked for. Return true (success).
        return requiredExtensions.empty(); 
    }

    // -------------------------------------------------------------------------
    // FUNCTION: isSuitable
    // PURPOSE:  The "Master Test" for a GPU. It runs all necessary checks 
    //           (like extensions, and later queue families) to see if we can use it.
    // -------------------------------------------------------------------------
    bool isSuitable(const vk::PhysicalDevice &device, bool debugMode)
    {
        if (debugMode)
        {
            std::cout << "Checking if device is suitable...\n";
        }

        // The Swapchain extension is mandatory if we want to actually draw things 
        // to a window on the screen (as opposed to computing math in the background).
        const std::vector<const char *> requestedExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        if(debugMode)
        {
            std::cout << "Requested Extensions: \n"; 
            for(const char* extension : requestedExtensions)
            {
                std::cout << "\t\"" << extension << "\"\n"; 
            }
        }

        // Call our helper function to check if the swapchain extension exists.
        bool extensionSupported = checkDeviceExtensionSupport(device, requestedExtensions, debugMode);

        // Evaluate the results
        if(extensionSupported)
        {
            if(debugMode)
            {
                std::cout << "Device CAN support the required extensions.\n"; 
            }
            // NOTE: In a full Vulkan engine, you would also check Queue Families 
            // and Swapchain details (formats, present modes) right here.
            
            // For now, if it has the extensions, we consider it a good GPU.
            return true; 
        }
        else
        {
            if(debugMode)
            {
                 std::cout << "Device CANNOT support the required extensions.\n"; 
            }
            return false; // GPU failed the test, reject it.
        }

        return true ; 
    }

    // -------------------------------------------------------------------------
    // FUNCTION: choose_physical_device
    // PURPOSE:  Finds all GPUs on the computer, tests them, and selects the best one.
    // -------------------------------------------------------------------------
    vk::PhysicalDevice choose_physical_device(vk::Instance &instance, bool debugMode)
    {
        // Ask the Vulkan Instance to find every Vulkan-compatible GPU installed.
        std::vector<vk::PhysicalDevice> availableDevices = instance.enumeratePhysicalDevices();
        
        if (debugMode)
        {
            std::cout << "\n--------------Physical Devices------------------------\n";
            std::cout << "Available Physical Devices -- " << availableDevices.size() << std::endl;
        }
        
        // Loop through every GPU we found.
        for (vk::PhysicalDevice device : availableDevices)
        {
            if (debugMode)
            {
                log_device_properties(device); // Print its name and type
            }

            // Run the hardware through our "Master Test".
            if (isSuitable(device, debugMode))
            {
                // The moment we find a device that passes all tests, we stop looking
                // and return it so our program can start using it.
                if (debugMode) std::cout << "Suitable device found!\n";
                return device; 
            }
        }

        // If the loop finishes and we haven't returned a device, it means NO GPU 
        // on this computer can run our program. We return nullptr to signal a crash/error.
        if (debugMode) std::cout << "Failed to find a suitable physical device.\n";
        return nullptr;
    }
} // namespace vkInit