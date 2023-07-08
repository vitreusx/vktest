#pragma once
#include <stdexcept>
#include <vktest/vulkan.h>

#define VK_CHECK(expr)                                                         \
  do {                                                                         \
    VkResult __result = (expr);                                                \
    if (__result != VK_SUCCESS)                                                \
      throw std::runtime_error(#expr);                                         \
  } while (false)