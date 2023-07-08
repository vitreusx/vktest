#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

void glfw_error_cb(int error_code, const char *desc) {
  std::cerr << "[GLFW] error_code = " << error_code << "; desc = \"" << desc
            << "\"" << std::endl;
}

static VkBool32 VKAPI_CALL create_debug_cb(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT *cb_data, void *user_ptr) {
  std::cerr << "[VkDebugUtils/" << (char const *)user_ptr << "]";
  switch (severity) {
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
    std::cerr << "[VERBOSE] ";
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
    std::cerr << "   [WARN] ";
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    std::cerr << "  [ERROR] ";
    break;
  };
  std::cerr << cb_data->pMessage << std::endl;
  return VK_FALSE;
}

int main() {
  if (glfwInit() != GLFW_TRUE)
    throw std::runtime_error("glfwInit");

  glfwSetErrorCallback(glfw_error_cb);

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  auto window = glfwCreateWindow(1280, 960, "vktest", nullptr, nullptr);
  if (!window)
    throw std::runtime_error("glfwCreateWindow");

  VULKAN_HPP_DEFAULT_DISPATCHER.init(glfwGetInstanceProcAddress);

  VkApplicationInfo app_info = {};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "vktest";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "vkengine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;
  app_info.pNext = nullptr;

  VkInstanceCreateInfo instance_info = {};
  instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instance_info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
  instance_info.pNext = nullptr;
  instance_info.pApplicationInfo = &app_info;

  std::vector<std::string> layers = {"VK_LAYER_KHRONOS_validation"};

  std::vector<char const *> layer_ptrs;
  for (auto &layer : layers)
    layer_ptrs.push_back(layer.c_str());
  instance_info.enabledLayerCount = (uint32_t)layer_ptrs.size();
  instance_info.ppEnabledLayerNames = layer_ptrs.data();

  std::vector<std::string> exts = {
      VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
      VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME};

  uint32_t glfw_ext_count = 0;
  auto **glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_ext_count);
  for (uint32_t i = 0; i < glfw_ext_count; ++i)
    exts.push_back(glfw_exts[i]);

  std::vector<char const *> ext_ptrs;
  for (auto &ext : exts)
    ext_ptrs.push_back(ext.c_str());
  instance_info.enabledExtensionCount = (uint32_t)ext_ptrs.size();
  instance_info.ppEnabledExtensionNames = ext_ptrs.data();

  VkDebugUtilsMessengerCreateInfoEXT create_debug_info = {};
  create_debug_info.sType =
      VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_debug_info.messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_debug_info.messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_debug_info.pfnUserCallback = create_debug_cb;
  create_debug_info.pNext = nullptr;
  create_debug_info.pUserData = "Create";
  create_debug_info.flags = 0;

  instance_info.pNext = &create_debug_info;

  VkInstance instance = {};
  auto result = vkCreateInstance(&instance_info, nullptr, &instance);
  if (result != VK_SUCCESS)
    throw std::runtime_error("vkCreateInstance");

  VULKAN_HPP_DEFAULT_DISPATCHER.init(instance, glfwGetInstanceProcAddress);

  auto normal_debug_info = create_debug_info;
  normal_debug_info.pUserData = "Normal";

  VkDebugUtilsMessengerEXT debug_messenger = {};
  result = vkCreateDebugUtilsMessengerEXT(instance, &normal_debug_info, nullptr,
                                          &debug_messenger);
  if (result != VK_SUCCESS)
    throw std::runtime_error("vkCreateDebugUtilsMessengerEXT");

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }

  vkDestroyDebugUtilsMessengerEXT(instance, debug_messenger, nullptr);
  vkDestroyInstance(instance, nullptr);

  glfwTerminate();

  return EXIT_SUCCESS;
}