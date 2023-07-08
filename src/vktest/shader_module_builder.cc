#include <vktest/shader_module_builder.h>

namespace vkt {
ShaderModuleBuilder::ShaderModuleBuilder() {
  info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
}

std::vector<char>
ShaderModuleBuilder::read_shader_file(std::filesystem::path const &path) {
  if (!std::filesystem::exists(path)) {
    std::stringstream err_ss;
    auto cwd = std::filesystem::current_path();
    err_ss << "shader file \"" << path.string() << "\" does not exist";
    throw std::runtime_error(err_ss.str());
  }

  std::ifstream shader_file(path, std::ios::ate | std::ios::binary);
  auto code_size = shader_file.tellg();
  std::vector<char> buf(code_size);
  shader_file.seekg(std::ios::beg);
  shader_file.read(buf.data(), code_size);
  return buf;
}

ShaderModuleBuilder &
ShaderModuleBuilder::set_shader_code(std::vector<char> code) {
  this->code = std::move(code);
  info.codeSize = this->code.size();
  info.pCode = reinterpret_cast<uint32_t const *>(this->code.data());
  return *this;
}

vkt::ShaderModule
ShaderModuleBuilder::build(std::shared_ptr<vkt::Device> dev) const {
  VkShaderModule module;
  VK_CHECK(vkCreateShaderModule(*dev, &info, nullptr, &module));
  return vkt::ShaderModule(std::move(module), std::move(dev));
}
} // namespace vkt