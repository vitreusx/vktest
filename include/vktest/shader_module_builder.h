#pragma once
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <vktest/builder.h>
#include <vktest/shader_module.h>
#include <vktest/utils/check.h>
#include <vktest/vulkan.h>

namespace vkt {
class ShaderModuleBuilder : public Builder<VkShaderModuleCreateInfo> {
public:
  ShaderModuleBuilder();

  static std::vector<char> read_shader_file(std::filesystem::path const &path);

  ShaderModuleBuilder &set_shader_code(std::vector<char> code);

  vkt::ShaderModule build(std::shared_ptr<vkt::Device> dev) const;

private:
  std::vector<char> code;
};
} // namespace vkt