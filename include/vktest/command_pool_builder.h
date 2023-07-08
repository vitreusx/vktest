#pragma once
#include <vktest/builder.h>
#include <vktest/command_pool.h>
#include <vktest/utils/check.h>
#include <vktest/vulkan.h>

namespace vkt {
class CommandPoolBuilder : public Builder<VkCommandPoolCreateInfo> {
public:
  CommandPoolBuilder();
  CommandPool build(std::shared_ptr<Device> dev);
};
} // namespace vkt