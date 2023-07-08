#include <vktest/shader_module.h>

namespace vkt {
ShaderModule::ShaderModule(VkShaderModule &&module,
                           std::shared_ptr<vkt::Device> dev) {
  this->data = std::move(module);
  this->dev = std::move(dev);
}

ShaderModule::~ShaderModule() {
  if (data)
    vkDestroyShaderModule(*dev, data, nullptr);
}
} // namespace vkt