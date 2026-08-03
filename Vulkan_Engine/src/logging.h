#pragma once
#include "config.h"

namespace vkInit
{
    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType, 
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        std::cerr << "Validation Layer - " << pCallbackData->pMessage << std::endl; 

        return VK_FALSE; 
    }

    vk::DebugUtilsMessengerEXT make_debug_messenger(vk::Instance& instance, vk::detail::DispatchLoaderDynamic& dldi)
    {
        vk::DebugUtilsMessengerCreateInfoEXT createInfo = vk::DebugUtilsMessengerCreateInfoEXT(
            vk::DebugUtilsMessengerCreateFlagsEXT(),
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
            // Cast to the vk-namespaced function pointer type to avoid the deprecation warning
            reinterpret_cast<vk::PFN_DebugUtilsMessengerCallbackEXT>(debugCallback),
            nullptr 
        );
        return instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dldi);
    }
}