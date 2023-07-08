#pragma once
#include <vktest/device.h>
#include <vktest/utils/check.h>
#include <vktest/utils/enumerate.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class QueueSubmitOp : public Wrapper<VkSubmitInfo> {
public:
  QueueSubmitOp();

  QueueSubmitOp &
  setWaitSemaphores(std::vector<std::pair<VkSemaphore, VkPipelineStageFlags>>
                        waitSemaphoreMaskPairs);

  QueueSubmitOp &setCommandBuffers(std::vector<VkCommandBuffer> commandBuffers);

  QueueSubmitOp &setSignalSemaphores(std::vector<VkSemaphore> signalSemaphores);

  VkQueue queue;
  VkFence fence;

  void operator()();

private:
  std::vector<std::pair<VkSemaphore, VkPipelineStageFlags>>
      waitSemaphoreMaskPairs;
  std::vector<VkSemaphore> waitSemaphores;
  std::vector<VkPipelineStageFlags> waitStageMasks;

  std::vector<VkCommandBuffer> commandBuffers;

  std::vector<VkSemaphore> signalSemaphores;
};

class QueuePresentOp : public Wrapper<VkPresentInfoKHR> {
public:
  QueuePresentOp();

  QueuePresentOp &setWaitSemaphores(std::vector<VkSemaphore> waitSemaphores);

  QueuePresentOp &setSwapchainImagePairs(
      std::vector<std::pair<VkSwapchainKHR, uint32_t>> swapchainImagePairs);

  VkQueue queue;

  void operator()();

private:
  std::vector<VkSemaphore> waitSemaphores;
  std::vector<std::pair<VkSwapchainKHR, uint32_t>> swapchainImagePairs;
  std::vector<VkSwapchainKHR> swapchains;
  std::vector<uint32_t> imageIndices;
  std::vector<VkResult> results;
};
} // namespace vkt