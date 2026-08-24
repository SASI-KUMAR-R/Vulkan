// File name : engine.cpp

#include "engine.h"
#include "config.h"
#include "instance.h"
#include "logging.h"
#include "device.h"

Engine::Engine()
{
    if (debugMode)
    {
        std::cout << "Graphics Vulkan\n";
    }

    build_glfw_window();

    make_instance();

    make_device();
}

void Engine::build_glfw_window()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    // Tell GLFW not to create an OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Disable window resizing
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Create the window
    window = glfwCreateWindow(
        width,
        height,
        "Vulkan",
        nullptr,
        nullptr);

    // Check if window creation succeeded
    if (window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window.");
    }

    if (debugMode)
    {
        std::cout << "Window created successfully.\n";
        std::cout << "Width  : " << width << '\n';
        std::cout << "Height : " << height << '\n';
    }
}

void Engine::make_instance()
{
    instance = vkInit::make_instance(debugMode, "Vulkan_Sasi");
    dldi = vk::detail::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);

    if (debugMode)
    {
        debugMessenger = vkInit::make_debug_messenger(instance, dldi);
    }

    VkSurfaceKHR c_style_surface;
    if (glfwCreateWindowSurface(instance, window, nullptr, &c_style_surface) != VK_SUCCESS)
    {
        if (debugMode)
        {
            std::cout << "Failed to Successfully connect to surface \n";
        }
    }
    else if (debugMode)
    {
        std::cout << "Successfully connect to surface \n";
    }

    surface = c_style_surface ; 
}

void Engine::make_device()
{
    physicalDevice = vkInit::choose_physical_device(instance, debugMode);
    device = vkInit::create_logical_device(physicalDevice, surface, debugMode);
    std::array<vk::Queue, 2> queues = vkInit::get_queue(physicalDevice, device, surface, debugMode);
    graphicsQueue = queues[0];
    presentQueue = queues[0];
    vkInit::query_swapchain_support(physicalDevice,surface,debugMode) ; 
}

Engine::~Engine()
{
    // 1. Destroy the logical device (and ideally, ensure the GPU is idle first)
    device.destroy();

    // 2. Destroy the surface BEFORE destroying the window it is attached to
    instance.destroySurfaceKHR(surface);

    // 3. Destroy the debug messenger
    if (debugMessenger)
    {
        instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
    }

    // 4. Destroy the Vulkan instance itself
    instance.destroy();

    // 5. NOW it is safe to destroy the OS window
    if (window != nullptr)
    {
        glfwDestroyWindow(window);
    }

    // 6. Terminate the windowing library
    glfwTerminate();

    if (debugMode)
    {
        std::cout << "Engine shutdown completed.\n";
    }
}