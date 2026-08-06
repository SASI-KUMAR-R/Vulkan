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

    make_device() ; 
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
}

void Engine::make_device()
{
    physicalDevice = vkInit::choose_physical_device(instance,debugMode) ; 
    vkInit::findQueueFamilies(physicalDevice,debugMode) ; 
}

Engine::~Engine()
{
    if (window != nullptr)
    {
        glfwDestroyWindow(window);
    }

    if (debugMode)
    {
        std::cout << "Engine shutdown completed.\n";
    }

    instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);

    instance.destroy();

    glfwTerminate();
}