#include <vktest/image_view_builder.h>
#include <vktest/utils/check.h>

namespace vkt {
ImageViewBuilder::ImageViewBuilder() {
  info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
}

ImageViewBuilder &ImageViewBuilder::setImage(VkImage image, VkFormat format) {
  info.image = image;
  info.format = format;
  return *this;
}

ImageViewBuilder &ImageViewBuilder::useColorTargetPreset() {
  info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
  info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  info.subresourceRange.baseMipLevel = 0;
  info.subresourceRange.levelCount = 1;
  info.subresourceRange.baseArrayLayer = 0;
  info.subresourceRange.layerCount = 1;
  return *this;
}

vkt::ImageView ImageViewBuilder::build(std::shared_ptr<vkt::Device> dev) const {
  VkImageView imageView;
  VK_CHECK(vkCreateImageView(*dev, &info, nullptr, &imageView));
  return vkt::ImageView(std::move(imageView), std::move(dev));
}
} // namespace vkt