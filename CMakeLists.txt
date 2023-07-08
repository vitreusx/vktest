cmake_minimum_required(VERSION 3.14)
project(vktest LANGUAGES C CXX)

add_executable(vktest src/main.cpp)

find_package(glfw3 3.3 QUIET)

if(NOT ${glfw3_FOUND})
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(ext/glfw)
endif()

find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)

target_link_libraries(vktest
    PRIVATE glfw OpenGL::GL Vulkan::Vulkan)

target_compile_features(vktest
    PUBLIC cxx_std_17)
