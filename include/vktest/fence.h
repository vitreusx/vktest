#pragma once
#include <vktest/utils/swap_shared_ptr.h>
#include <vktest/device.h>
#include <vktest/utils/check.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class Fence : public Wrapper<VkFence> {
public:
  Fence() = default;
  Fence(std::shared_ptr<vkt::Device> dev, VkFenceCreateFlags flags = 0);
  ~Fence();

  Fence(Fence &&) = default;
  Fence &operator=(Fence &&) = default;

private:
  vkt::swap_shared_ptr<vkt::Device> dev;
};
} // namespace vkt