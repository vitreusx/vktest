cmake_minimum_required(VERSION 3.19)
project(vktest LANGUAGES C CXX)

file(GLOB_RECURSE Headers CONFIGURE_DEPENDS
    include/vktest/*.h)

file(GLOB_RECURSE Sources CONFIGURE_DEPENDS
    src/vktest/*.cc src/vktest/*.cpp)

add_library(vktest)

target_sources(vktest
    PUBLIC ${Headers}
    PRIVATE ${Sources})

target_include_directories(vktest
    PUBLIC include
    PRIVATE src)

find_package(glfw3 3.3 QUIET)

if(NOT ${glfw3_FOUND})
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(ext/glfw)
endif()

find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)

set(VK_EXTENSIONS
    VK_KHR_swapchain
    VK_EXT_debug_utils
    VK_KHR_portability_enumeration
    VK_KHR_win32_surface
    VK_KHR_surface)

string(REPLACE ";" "," VK_EXTENSIONS_STR "${VK_EXTENSIONS}")

add_subdirectory(ext/glad/cmake)
glad_add_library(glad_vk
    REPRODUCIBLE LOADER
    API vulkan=1.0
    EXTENSIONS "${VK_EXTENSIONS_STR}")

target_link_libraries(vktest
    PUBLIC glfw OpenGL::GL Vulkan::Vulkan glad_vk)

target_compile_features(vktest
    PUBLIC cxx_std_17)

add_executable(example src/main.cc)
target_link_libraries(example PRIVATE vktest)

function(add_spirv_shader INPUT_FILE OUTPUT_FILE)
    cmake_path(GET OUTPUT_FILE PARENT_PATH OUTPUT_PARENT)
    add_custom_command(
        OUTPUT "${OUTPUT_FILE}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${OUTPUT_PARENT}"
        COMMAND "${Vulkan_GLSLC_EXECUTABLE}" "${INPUT_FILE}" -o "${OUTPUT_FILE}"
        MAIN_DEPENDENCY "${INPUT_FILE}"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endfunction()

set(ShadersDir "${CMAKE_CURRENT_LIST_DIR}/src/shaders")

file(GLOB_RECURSE ShaderFiles
    CONFIGURE_DEPENDS
    RELATIVE "${ShadersDir}"
    "${ShadersDir}/**")

set(CompiledShaders)

set(CompiledShadersDir "${CMAKE_CURRENT_BINARY_DIR}/shaders")

foreach(ShaderFile IN LISTS ShaderFiles)
    set(CompiledShader "${CompiledShadersDir}/${ShaderFile}.spv")
    add_spirv_shader("${ShadersDir}/${ShaderFile}" "${CompiledShader}")
    list(APPEND CompiledShaders "${CompiledShader}")
endforeach()

add_custom_target(shaders DEPENDS "${CompiledShaders}")
add_dependencies(example shaders)

add_custom_command(
    TARGET example
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CompiledShadersDir}" "$<TARGET_FILE_DIR:example>/shaders"
    COMMENT "Copying shaders next to executable")