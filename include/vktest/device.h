#pragma once
#include <vector>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class Device : public Wrapper<VkDevice> {
public:
  explicit Device(VkDevice &&device);
  virtual ~Device();

  Device(Device &&) = default;
  Device &operator=(Device &&) = default;

  VkQueue getQueue(uint32_t familyIndex, uint32_t queueIndex) const;
};
} // namespace vkt