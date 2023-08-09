#pragma once
#include <vktest/device.h>
#include <vktest/utils/swap_shared_ptr.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>


namespace vkt {
class Pipeline : public Wrapper<VkPipeline> {
public:
  Pipeline() = default;
  Pipeline(VkPipeline &&data, std::shared_ptr<vkt::Device> dev);
  ~Pipeline();

  Pipeline(Pipeline &&) = default;
  Pipeline &operator=(Pipeline &&) = default;

private:
  vkt::swap_shared_ptr<vkt::Device> dev;
};
} // namespace vkt