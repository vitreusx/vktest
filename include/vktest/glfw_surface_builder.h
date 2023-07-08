#pragma once
#include <vktest/builder.h>
#include <vktest/glfw_window.h>
#include <vktest/surface.h>
#include <vktest/vulkan.h>

namespace vkt {
class GLFWSurfaceBuilder {
public:
  GLFWSurfaceBuilder &setWindow(GLFWWindow &window);

  Surface build(std::shared_ptr<vkt::Instance> instance) const;

private:
  GLFWWindow *window = nullptr;
};
} // namespace vkt