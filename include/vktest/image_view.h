#pragma once
#include <memory>
#include <vktest/device.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class ImageView : public Wrapper<VkImageView> {
public:
  ImageView(VkImageView &&view, std::shared_ptr<vkt::Device> dev);
  ~ImageView();

  ImageView(ImageView &&) = default;
  ImageView &operator=(ImageView &&) = default;

private:
  std::shared_ptr<vkt::Device> dev;
};
} // namespace vkt