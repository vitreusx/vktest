#pragma once
#include <memory>
#include <vktest/device.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class RenderPass : public Wrapper<VkRenderPass> {
public:
  RenderPass() = default;
  RenderPass(VkRenderPass &&data, std::shared_ptr<vkt::Device> dev);
  ~RenderPass();

  RenderPass(RenderPass &&) = default;
  RenderPass &operator=(RenderPass &&) = default;

private:
  std::shared_ptr<vkt::Device> dev;
};
} // namespace vkt