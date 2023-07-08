#include <vktest/glfw_surface_builder.h>
#include <vktest/utils/check.h>

namespace vkt {
GLFWSurfaceBuilder &GLFWSurfaceBuilder::setWindow(GLFWWindow &window) {
  this->window = &window;
  return *this;
}

Surface
GLFWSurfaceBuilder::build(std::shared_ptr<vkt::Instance> instance) const {
  VkSurfaceKHR surface;
  auto result = glfwCreateWindowSurface(*instance, *window, nullptr, &surface);
  if (result != VK_SUCCESS)
    throw std::runtime_error("glfwCreateWindowSurface");
  return Surface(std::move(surface), std::move(instance));
}
} // namespace vkt