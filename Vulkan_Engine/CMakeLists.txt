cmake_minimum_required(VERSION 3.16)

project(Vulkan_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(Vulkan_Engine
    src/main.cpp
    src/engine.cpp
)

target_link_libraries(Vulkan_Engine PRIVATE
    Vulkan::Vulkan
    glfw
)

target_include_directories(Vulkan_Engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)