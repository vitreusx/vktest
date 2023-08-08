#pragma once
#include <sstream>
#include <stdexcept>
#include <vktest/vulkan.h>

#define VK_CHECK(expr)                                                         \
  do {                                                                         \
    VkResult __result = (expr);                                                \
    if (__result != VK_SUCCESS) {                                              \
      std::stringstream error_ss;                                              \
      error_ss << "Vulkan error [Code: " << __result << "] for \"" << #expr    \
               << "\"";                                                        \
      throw std::runtime_error(error_ss.str());                                \
    }                                                                          \
  } while (false)
