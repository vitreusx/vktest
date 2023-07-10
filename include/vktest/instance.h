#pragma once
#include <optional>
#include <stdexcept>
#include <vector>
#include <vktest/debug_messenger_builder.h>
#include <vktest/phys_dev.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class Instance : public Wrapper<VkInstance> {
public:
  explicit Instance(VkInstance &&instance,
                    std::optional<Callback<DebugCbSig>> debugCb);
  virtual ~Instance();

  Instance(Instance &&) = default;
  Instance &operator=(Instance &&) = default;

  std::vector<vkt::PhysicalDevice> getPhysicalDevices() const;

private:
  std::optional<Callback<DebugCbSig>> debugCb;
};
} // namespace vkt