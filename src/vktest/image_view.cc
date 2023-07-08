#include <vktest/image_view.h>

namespace vkt {
ImageView::ImageView(VkImageView &&view, std::shared_ptr<vkt::Device> dev) {
  this->data = std::move(view);
  this->dev = std::move(dev);
}

ImageView::~ImageView() {
  if (data)
    vkDestroyImageView(*dev, data, nullptr);
}
} // namespace vkt