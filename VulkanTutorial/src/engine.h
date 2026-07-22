
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

    // glfw setup
    void build_glfw_window();

    // vulkan instance
    vk::Instance instance{nullptr};

    //instance setup
    void make_instance(); 
};