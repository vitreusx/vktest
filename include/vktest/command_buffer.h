#pragma once
#include <memory>
#include <vector>
#include <vktest/utils/check.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class RenderPassBeginOp : public Wrapper<VkRenderPassBeginInfo> {
public:
  RenderPassBeginOp(VkCommandBuffer cmdBuf);

  RenderPassBeginOp(RenderPassBeginOp const &) = default;
  RenderPassBeginOp &operator=(RenderPassBeginOp const &) = default;

  RenderPassBeginOp(RenderPassBeginOp &&) = default;
  RenderPassBeginOp &operator=(RenderPassBeginOp &&) = default;

  RenderPassBeginOp &setClearValues(std::vector<VkClearValue> clearValues);

  void operator()() const;

  VkSubpassContents subpassContents;

private:
  VkCommandBuffer cmdBuf;
  std::vector<VkClearValue> clearValues;
};

class CommandBuffer : public Wrapper<VkCommandBuffer> {
public:
  CommandBuffer();
  CommandBuffer(VkCommandBuffer &&data);

  CommandBuffer(CommandBuffer &&) = default;
  CommandBuffer &operator=(CommandBuffer &&) = default;

  void startRecording();

  RenderPassBeginOp beginRenderPass;
  void endRenderPass();
};
} // namespace vkt