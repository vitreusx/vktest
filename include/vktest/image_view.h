#pragma once
#include <vktest/device.h>
#include <vktest/utils/swap_shared_ptr.h>
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
  vkt::swap_shared_ptr<vkt::Device> dev;
};
} // namespace vkt