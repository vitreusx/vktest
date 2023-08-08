#include <vktest/queue.h>

namespace vkt {
QueueSubmitOp::QueueSubmitOp() {
  data.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
}

QueueSubmitOp &QueueSubmitOp::setWaitSemaphores(
    std::vector<std::pair<VkSemaphore, VkPipelineStageFlags>>
        waitSemaphoreMaskPairs) {
  this->waitSemaphoreMaskPairs = std::move(waitSemaphoreMaskPairs);
  auto count = this->waitSemaphoreMaskPairs.size();
  data.waitSemaphoreCount = (uint32_t)count;

  waitSemaphores.resize(count);
  waitStageMasks.resize(count);

  for (auto const &[idx, semMaskPair] :
       enumerate(this->waitSemaphoreMaskPairs)) {
    auto const &[semaphore, mask] = semMaskPair;
    waitSemaphores[idx] = semaphore;
    waitStageMasks[idx] = mask;
  }

  data.pWaitSemaphores = waitSemaphores.data();
  data.pWaitDstStageMask = waitStageMasks.data();

  return *this;
}

QueueSubmitOp &
QueueSubmitOp::setCommandBuffers(std::vector<VkCommandBuffer> commandBuffers) {
  this->commandBuffers = std::move(commandBuffers);
  data.commandBufferCount = (uint32_t)this->commandBuffers.size();
  data.pCommandBuffers = this->commandBuffers.data();
  return *this;
}

QueueSubmitOp &
QueueSubmitOp::setSignalSemaphores(std::vector<VkSemaphore> signalSemaphores) {
  this->signalSemaphores = std::move(signalSemaphores);
  data.signalSemaphoreCount = (uint32_t)this->signalSemaphores.size();
  data.pSignalSemaphores = this->signalSemaphores.data();
  return *this;
}

VkResult QueueSubmitOp::operator()() {
  return vkQueueSubmit(queue, 1, &data, fence);
}

QueuePresentOp::QueuePresentOp() {
  data.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
}

QueuePresentOp &
QueuePresentOp::setWaitSemaphores(std::vector<VkSemaphore> waitSemaphores) {
  this->waitSemaphores = std::move(waitSemaphores);
  data.waitSemaphoreCount = (uint32_t)this->waitSemaphores.size();
  data.pWaitSemaphores = this->waitSemaphores.data();
  return *this;
}

QueuePresentOp &QueuePresentOp::setSwapchainImagePairs(
    std::vector<std::pair<VkSwapchainKHR, uint32_t>> swapchainImagePairs) {
  this->swapchainImagePairs = std::move(swapchainImagePairs);
  auto swapchainCount = this->swapchainImagePairs.size();
  data.swapchainCount = (uint32_t)swapchainCount;

  swapchains.resize(swapchainCount);
  imageIndices.resize(swapchainCount);
  results.resize(swapchainCount);

  for (auto const &[idx, swapchainImagePair] :
       enumerate(this->swapchainImagePairs)) {
    auto const &[swapchain, imageIdx] = swapchainImagePair;
    swapchains[idx] = swapchain;
    imageIndices[idx] = imageIdx;
  }

  data.pSwapchains = swapchains.data();
  data.pImageIndices = imageIndices.data();
  data.pResults = results.data();
  return *this;
}

VkQueue queue;

VkResult QueuePresentOp::operator()() {
  return vkQueuePresentKHR(queue, &data);
}
} // namespace vkt