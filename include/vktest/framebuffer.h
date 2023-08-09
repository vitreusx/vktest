#pragma once
#include <vktest/utils/swap_shared_ptr.h>
#include <vktest/device.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class Framebuffer : public Wrapper<VkFramebuffer> {
public:
  Framebuffer(VkFramebuffer &&data, std::shared_ptr<vkt::Device> dev);
  ~Framebuffer();

  Framebuffer(Framebuffer &&) = default;
  Framebuffer &operator=(Framebuffer &&) = default;

private:
  vkt::swap_shared_ptr<vkt::Device> dev;
};
} // namespace vkt