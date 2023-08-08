#pragma once
#include <memory>
#include <vector>
#include <vktest/command_buffer.h>
#include <vktest/device.h>
#include <vktest/utils/check.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class CommandPool : public Wrapper<VkCommandPool> {
public:
  CommandPool() = default;
  CommandPool(VkCommandPool &&data, std::shared_ptr<vkt::Device> dev);
  ~CommandPool();

  CommandPool(CommandPool &&) = default;
  CommandPool &operator=(CommandPool &&) = default;

  std::vector<CommandBuffer> allocateBuffers(uint32_t count,
                                             VkCommandBufferLevel level);

  CommandBuffer allocateBuffer(VkCommandBufferLevel level);

private:
  std::shared_ptr<vkt::Device> dev;
};
} // namespace vkt