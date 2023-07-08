#pragma once
#include <vktest/vulkan.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <vector>
#include <vktest/cb.h>

namespace vkt {
class GLFWSys {
public:
  static std::shared_ptr<GLFWSys> getInstance();
  ~GLFWSys();

  Callback<GLFWerrorfun> onError;

  std::vector<std::string> getVkInstanceExtensions() const;

private:
  GLFWSys();
};
} // namespace vkt