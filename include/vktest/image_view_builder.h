#pragma once
#include <vktest/builder.h>
#include <vktest/image_view.h>
#include <vktest/vulkan.h>

namespace vkt {
class ImageViewBuilder : public Builder<VkImageViewCreateInfo> {
public:
  ImageViewBuilder();

  ImageViewBuilder &setImage(VkImage image, VkFormat format);
  ImageViewBuilder &useColorTargetPreset();

  vkt::ImageView build(std::shared_ptr<vkt::Device> dev) const;
};
} // namespace vkt