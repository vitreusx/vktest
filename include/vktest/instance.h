#pragma once
#include <stdexcept>
#include <vector>
#include <vktest/phys_dev.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class Instance : public Wrapper<VkInstance> {
public:
  explicit Instance(VkInstance &&instance);
  virtual ~Instance();

  Instance(Instance &&) = default;
  Instance &operator=(Instance &&) = default;

  std::vector<vkt::PhysicalDevice> getPhysicalDevices() const;
};
} // namespace vkt