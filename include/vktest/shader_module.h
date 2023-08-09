#pragma once
#include <vktest/device.h>
#include <vktest/utils/swap_shared_ptr.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>


namespace vkt {
class ShaderModule : public Wrapper<VkShaderModule> {
public:
  ShaderModule(VkShaderModule &&module, std::shared_ptr<vkt::Device> dev);
  ~ShaderModule();

  ShaderModule(ShaderModule &&) = default;
  ShaderModule &operator=(ShaderModule &&) = default;

private:
  vkt::swap_shared_ptr<vkt::Device> dev;
};
} // namespace vkt