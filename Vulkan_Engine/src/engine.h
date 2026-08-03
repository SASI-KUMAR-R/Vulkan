
#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "config.h"

class Engine
{
public:
    Engine();
    ~Engine();

private:
    bool debugMode = true;

    int width{650};
    int height{480};

    GLFWwindow *window{nullptr};

    // instance - related variables
    vk::Instance instance{nullptr};
    vk::DebugUtilsMessengerEXT debugMessenger{nullptr};
    vk::detail::DispatchLoaderDynamic dldi;

    // instance setup
    void make_instance();

    // glfw setup
    void build_glfw_window();

    // device setup 
    void make_device() ; 

};