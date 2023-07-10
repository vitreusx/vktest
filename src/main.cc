#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <optional>
#include <sstream>
#include <unordered_set>
#include <vktest/app_info.h>
#include <vktest/avail_device_exts.h>
#include <vktest/avail_exts.h>
#include <vktest/avail_layers.h>
#include <vktest/command_pool_builder.h>
#include <vktest/debug_messenger_builder.h>
#include <vktest/device_builder.h>
#include <vktest/fence.h>
#include <vktest/framebuffer_builder.h>
#include <vktest/glfw_surface_builder.h>
#include <vktest/glfw_window.h>
#include <vktest/image_view_builder.h>
#include <vktest/instance_builder.h>
#include <vktest/phys_dev.h>
#include <vktest/pipeline_builder.h>
#include <vktest/pipeline_layout_builder.h>
#include <vktest/queue.h>
#include <vktest/render_pass_builder.h>
#include <vktest/semaphore.h>
#include <vktest/shader_module_builder.h>
#include <vktest/swapchain_builder.h>
#include <vktest/utils/check.h>
#include <vktest/utils/enumerate.h>
#include <vktest/vk_sys.h>

#undef min
#undef max

class Main {
public:
  int operator()() {
    setup();
    renderLoop();

    vkDeviceWaitIdle(*dev);
    return EXIT_SUCCESS;
  }

  void setup() {
    setupGlfw();
    setupWindow();
    setupVulkan();
    setLayersAndExts();
    createInstance();
    createWindowSurface();
    findSuitableDevice();
    createDevice();
    createSwapchain();
    createSwapchainImageViews();
    createShaders();
    createRenderPass();
    createPipeline();
    createFramebuffers();
    createSync();
    createCommand();
  }

  void setupGlfw() {
    glfwSys = vkt::GLFWSys::getInstance();

    glfwSys->onError = [](int errorCode, char const *description) {
      std::cout << "GLFW error: (" << errorCode << ") \"" << description
                << "\"";
    };
  }

  void setupWindow() {
    auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window = std::make_unique<vkt::GLFWWindow>(
        *glfwSys, mode->width / 2, mode->height / 2, "vktest", GLFW_NO_API);

    window->on->keyEvent = [this](int key, int scancode, int action, int mods) {
      if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
        glfwSetWindowShouldClose(*window, GLFW_TRUE);
    };
  }

  void setupVulkan() {
    vkSys = std::make_unique<vkt::VkSys>();
    vkSys->load();
  }

  void setLayersAndExts() {
    auto vkGlfwExts = glfwSys->getVkInstanceExtensions();
    customExts = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
                  VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME};
    customExts.insert(customExts.end(), vkGlfwExts.begin(), vkGlfwExts.end());

    customLayers = {"VK_LAYER_KHRONOS_validation"};
  }

  void createInstance() {
    vkt::AppInfo appInfo;
    appInfo.setAppName("vktest");

    vkt::InstanceBuilder instanceBuilder;
    instanceBuilder.setAppInfo(appInfo);
    instanceBuilder.setFlags(VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR);

    vkt::AvailExts availExts;
    for (auto const &ext : customExts) {
      if (!availExts.isSupported(ext)) {
        std::stringstream error_ss;
        error_ss << "VK extension \"" << ext << "\" is not supported.";
        throw std::runtime_error(error_ss.str());
      }
    }

    instanceBuilder.setExtensions(customExts);

    vkt::AvailLayers availLayers;
    for (auto const &layer : customLayers) {
      if (!availLayers.isSupported(layer)) {
        std::stringstream error_ss;
        error_ss << "VK layer \"" << layer << "\" is not supported.";
        throw std::runtime_error(error_ss.str());
      }
    }

    instanceBuilder.setLayers(customLayers);

    prepareDebuggerBuilder();
    instanceBuilder.enableDebugger(debuggerBuilder);

    instance = std::make_shared<vkt::Instance>(instanceBuilder.build());
    vkSys->load(*instance);

    debugger =
        std::make_unique<vkt::DebugMessenger>(debuggerBuilder.build(instance));
  }

  void prepareDebuggerBuilder() {
    debuggerBuilder.setSeverity(
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);
    debuggerBuilder.setMessageType(
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT);

    debuggerBuilder.setCallback(
        [](VkDebugUtilsMessageSeverityFlagBitsEXT severity,
           VkDebugUtilsMessageTypeFlagsEXT type,
           const VkDebugUtilsMessengerCallbackDataEXT *cbData) -> VkBool32 {
          std::cout << "[Validation Layer] " << cbData->pMessage << std::endl;
          return VK_FALSE;
        });
  }

  void createWindowSurface() {
    vkt::GLFWSurfaceBuilder surfaceBuilder;
    surfaceBuilder.setWindow(*window);
    surf = std::make_unique<vkt::Surface>(surfaceBuilder.build(instance));
  }

  void findSuitableDevice() {
    physDevs = instance->getPhysicalDevices();

    reqDevExts = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    for (auto &phys_dev : physDevs) {
      chosenDev = selectDevice(phys_dev);
      if (chosenDev.has_value())
        break;
    }

    if (!chosenDev.has_value())
      throw std::runtime_error("No suitable device found");
  }

  struct ChosenDev {
    vkt::PhysicalDevice *dev;
    uint32_t graphicsQueueIdx, presentQueueIdx;
    vkt::SurfaceDetails surfaceDetails;
  };

  std::optional<ChosenDev> selectDevice(vkt::PhysicalDevice &physDev) {
    ChosenDev chosenDev;
    chosenDev.dev = &physDev;

    auto isDiscrete =
        physDev.deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
    if (!isDiscrete)
      return std::nullopt;

    auto hasGeomShader = physDev.features.geometryShader;
    if (!hasGeomShader)
      return std::nullopt;

    std::optional<uint32_t> graphicsQueueIndex;
    for (auto const &[idx, prop] : vkt::enumerate(physDev.queueFamilyProps)) {
      if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        graphicsQueueIndex = (uint32_t)idx;
        break;
      }
    }

    if (!graphicsQueueIndex.has_value())
      return std::nullopt;
    else
      chosenDev.graphicsQueueIdx = graphicsQueueIndex.value();

    std::optional<uint32_t> presentQueueIndex;
    for (uint32_t qfIndex = 0; qfIndex < physDev.queueFamilyProps.size();
         ++qfIndex) {
      if (!glfwGetPhysicalDevicePresentationSupport(*instance, physDev,
                                                    qfIndex))
        continue;

      VkBool32 surfaceSupport;
      VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(physDev, qfIndex, *surf,
                                                    &surfaceSupport));

      if (!surfaceSupport)
        continue;

      presentQueueIndex = qfIndex;
      break;
    }

    if (!presentQueueIndex.has_value())
      return std::nullopt;
    else
      chosenDev.presentQueueIdx = presentQueueIndex.value();

    vkt::AvailDeviceExts availDevExts(physDev);
    for (auto const &devExt : reqDevExts)
      if (!availDevExts.isSupported(devExt))
        return std::nullopt;

    auto surfaceDetails = surf->getDetails(physDev);
    if (surfaceDetails.formats.empty() || surfaceDetails.presentModes.empty())
      return std::nullopt;

    chosenDev.surfaceDetails = surfaceDetails;

    return chosenDev;
  }

  void createDevice() {
    std::unordered_set<uint32_t> familyIndicesSet = {
        chosenDev->graphicsQueueIdx, chosenDev->presentQueueIdx};
    std::vector<uint32_t> familyIndices(familyIndicesSet.begin(),
                                        familyIndicesSet.end());

    vkt::DeviceBuilder deviceBuilder;
    deviceBuilder.setExts(reqDevExts);
    deviceBuilder.setLayers(customLayers);

    std::vector<vkt::DeviceQueueInfo> queueInfos(familyIndices.size());
    for (auto const &[idx, queueFamilyIdx] : vkt::enumerate(familyIndices)) {
      queueInfos[idx].setFamilyIndex(queueFamilyIdx);
      queueInfos[idx].setCount(1);
    }
    deviceBuilder.setQueueInfos(queueInfos);

    dev = std::make_shared<vkt::Device>(deviceBuilder.build(*chosenDev->dev));
    vkSys->load(*instance, *chosenDev->dev, *dev);

    graphicsQueue = dev->getQueue(chosenDev->graphicsQueueIdx, 0);
    presentQueue = dev->getQueue(chosenDev->presentQueueIdx, 0);
  }

  void createSwapchain() {
    swapchainBuilder->surface = *surf;
    swapchainBuilder->imageArrayLayers = 1;
    swapchainBuilder->imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainBuilder->compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainBuilder->preTransform =
        chosenDev->surfaceDetails.capabilities.currentTransform;
    swapchainBuilder->clipped = VK_TRUE;

    std::optional<VkSurfaceFormatKHR> format;
    for (auto const &availFormat : chosenDev->surfaceDetails.formats) {
      if (availFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
          availFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        format = availFormat;
        break;
      }
    }

    if (!format.has_value()) {
      throw std::runtime_error("No suitable format found");
    } else {
      swapchainBuilder->imageFormat = format.value().format;
      swapchainBuilder->imageColorSpace = format.value().colorSpace;
    }

    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (auto const &avail_mode : chosenDev->surfaceDetails.presentModes) {
      if (avail_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
        presentMode = avail_mode;
        break;
      }
    }
    swapchainBuilder->presentMode = presentMode;

    auto &capabilities = chosenDev->surfaceDetails.capabilities;
    if (capabilities.currentExtent.width !=
        std::numeric_limits<uint32_t>::max()) {
      extent = capabilities.currentExtent;
    } else {
      int width, height;
      glfwGetFramebufferSize(*window, &width, &height);
      extent.width =
          std::clamp((uint32_t)width, capabilities.minImageExtent.width,
                     capabilities.maxImageExtent.width);
      extent.height =
          std::clamp((uint32_t)height, capabilities.minImageExtent.height,
                     capabilities.maxImageExtent.height);
    }
    swapchainBuilder->imageExtent = extent;

    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0)
      imageCount = std::min(imageCount, capabilities.maxImageCount);
    swapchainBuilder->minImageCount = imageCount;

    std::unordered_set<uint32_t> familyIndicesSet = {
        chosenDev->graphicsQueueIdx, chosenDev->presentQueueIdx};
    std::vector<uint32_t> familyIndices(familyIndicesSet.begin(),
                                        familyIndicesSet.end());

    if (familyIndices.size() == 1) {
      swapchainBuilder->imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      swapchainBuilder.setQueueFamilyIndices({});
    } else {
      swapchainBuilder->imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      swapchainBuilder.setQueueFamilyIndices(familyIndices);
    }

    swapchain = std::make_unique<vkt::Swapchain>(swapchainBuilder.build(dev));
  }

  void createSwapchainImageViews() {
    vkt::ImageViewBuilder imageViewBuilder;
    imageViewBuilder.useColorTargetPreset();

    imageViews.clear();
    for (auto const &image : swapchain->getImages()) {
      imageViewBuilder.setImage(image, swapchainBuilder->imageFormat);
      imageViews.emplace_back(imageViewBuilder.build(dev));
    }
  }

  void createShaders() {
    auto shaderBuilder = vkt::ShaderModuleBuilder();

    auto vertShaderCode = vkt::ShaderModuleBuilder::read_shader_file(
        "shaders/triangle/shader.vert.spv");
    shaderBuilder.set_shader_code(vertShaderCode);
    vertShader = std::make_shared<vkt::ShaderModule>(shaderBuilder.build(dev));

    auto fragShaderCode = vkt::ShaderModuleBuilder::read_shader_file(
        "shaders/triangle/shader.frag.spv");
    shaderBuilder.set_shader_code(fragShaderCode);
    fragShader = std::make_shared<vkt::ShaderModule>(shaderBuilder.build(dev));
  }

  void createRenderPass() {
    vkt::RenderPassBuilder renderPassBuilder;

    VkAttachmentDescription colorAtt = {};
    colorAtt.format = swapchainBuilder->imageFormat;
    colorAtt.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAtt.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAtt.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAtt.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAtt.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAtt.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAtt.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    renderPassBuilder.setAttachments({colorAtt});
    auto colorAttRef =
        VkAttachmentReference{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

    vkt::RenderSubpass subpass;
    subpass->pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.setColorAttachments({colorAttRef});

    renderPassBuilder.setSubpasses({subpass});

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    renderPassBuilder.setDependencies({dependency});

    renderPass =
        std::make_unique<vkt::RenderPass>(renderPassBuilder.build(dev));
  }

  void createPipeline() {
    vkt::PipelineBuilder pipelineBuilder;

    pipelineBuilder.setShaderStages(
        {vkt::ShaderStage(vertShader, VK_SHADER_STAGE_VERTEX_BIT, "main"),
         vkt::ShaderStage(fragShader, VK_SHADER_STAGE_FRAGMENT_BIT, "main")});

    pipelineBuilder.setDynamicStates(
        {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR});

    viewport = {0.0f, 0.0f, (float)extent.width, (float)extent.height,
                0.0f, 1.0f};
    pipelineBuilder.setViewports({viewport});

    scissor = {{0, 0}, extent};
    pipelineBuilder.setScissors({scissor});

    auto &rasterizer = pipelineBuilder.rasterizerInfo;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    auto &multisampling = pipelineBuilder.multisamplingInfo;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    auto &blending = pipelineBuilder.colorBlendInfo;
    blending.logicOpEnable = VK_FALSE;
    blending.logicOp = VK_LOGIC_OP_COPY;
    blending.blendConstants[0] = blending.blendConstants[1] =
        blending.blendConstants[2] = blending.blendConstants[3] = 0.0f;

    VkPipelineColorBlendAttachmentState colorBlendAtt;
    colorBlendAtt.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAtt.blendEnable = VK_FALSE;
    colorBlendAtt.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAtt.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAtt.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAtt.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAtt.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAtt.alphaBlendOp = VK_BLEND_OP_ADD;
    // colorBlendAtt.blendEnable = VK_TRUE;
    // colorBlendAtt.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    // colorBlendAtt.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    // colorBlendAtt.colorBlendOp = VK_BLEND_OP_ADD;
    // colorBlendAtt.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    // colorBlendAtt.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    // colorBlendAtt.alphaBlendOp = VK_BLEND_OP_ADD;

    pipelineBuilder.setColorBlendAttachments({colorBlendAtt});

    vkt::PipelineLayoutBuilder pipelineLayoutBuilder;
    auto pipelineLayout = pipelineLayoutBuilder.build(dev);
    pipelineBuilder.setLayout(pipelineLayout);

    pipelineBuilder.setRenderPass(*renderPass, 0);

    pipeline = std::make_unique<vkt::Pipeline>(pipelineBuilder.build(dev));
  }

  void createFramebuffers() {
    vkt::FramebufferBuilder fbBuilder;
    fbBuilder.setExtent(extent);
    fbBuilder.setRenderPass(*renderPass);

    for (auto const &imageView : imageViews) {
      fbBuilder.attachImageViews({(VkImageView)imageView});
      framebuffers.emplace_back(fbBuilder.build(dev));
    }
  }

  void createSync() {
    imageAvailSem = std::make_unique<vkt::Semaphore>(dev);
    renderFinishedSem = std::make_unique<vkt::Semaphore>(dev);
    inFlightFence =
        std::make_unique<vkt::Fence>(dev, VK_FENCE_CREATE_SIGNALED_BIT);
  }

  void createCommand() {
    vkt::CommandPoolBuilder cmdPoolBuilder;
    cmdPoolBuilder->flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    cmdPoolBuilder->queueFamilyIndex = chosenDev->graphicsQueueIdx;

    cmdPool = std::make_unique<vkt::CommandPool>(cmdPoolBuilder.build(dev));
    cmdBuf = std::make_unique<vkt::CommandBuffer>(
        cmdPool->allocateBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY));

    cmdBuf->beginRenderPass->renderPass = *renderPass;
    cmdBuf->beginRenderPass->framebuffer = framebuffers[0];
    cmdBuf->beginRenderPass->renderArea.offset = {0, 0};
    cmdBuf->beginRenderPass->renderArea.extent = extent;
    auto clearValue = VkClearValue{{{0.0f, 0.0f, 0.0f, 1.0f}}};
    cmdBuf->beginRenderPass.setClearValues({clearValue});
    cmdBuf->beginRenderPass.subpassContents = VK_SUBPASS_CONTENTS_INLINE;

    queueSubmit.setCommandBuffers({(VkCommandBuffer)*cmdBuf});
    queueSubmit.setWaitSemaphores(
        {{(VkSemaphore)*imageAvailSem,
          VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}});
    queueSubmit.setSignalSemaphores({(VkSemaphore)*renderFinishedSem});
    queueSubmit.fence = *inFlightFence;
    queueSubmit.queue = graphicsQueue;

    present.queue = presentQueue;
    present.setWaitSemaphores({(VkSemaphore)*renderFinishedSem});
  }

  void renderLoop() {
    while (!glfwWindowShouldClose(*window)) {
      glfwPollEvents();

      vkWaitForFences(*dev, 1, &inFlightFence->base(), VK_TRUE, UINT64_MAX);
      vkResetFences(*dev, 1, &inFlightFence->base());

      uint32_t imageIndex;
      vkAcquireNextImageKHR(*dev, *swapchain, UINT64_MAX, *imageAvailSem,
                            nullptr, &imageIndex);
      cmdBuf->beginRenderPass->framebuffer = framebuffers[imageIndex];

      vkResetCommandBuffer(*cmdBuf, 0);
      cmdBuf->startRecording();
      cmdBuf->beginRenderPass();
      vkCmdBindPipeline(*cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline);
      vkCmdSetViewport(*cmdBuf, 0, 1, &viewport);
      vkCmdSetScissor(*cmdBuf, 0, 1, &scissor);
      vkCmdDraw(*cmdBuf, 3, 1, 0, 0);
      cmdBuf->endRenderPass();
      VK_CHECK(vkEndCommandBuffer(*cmdBuf));

      queueSubmit();

      present.setSwapchainImagePairs(
          {{(VkSwapchainKHR)*swapchain, imageIndex}});
      present();
    }
  }

private:
  std::shared_ptr<vkt::GLFWSys> glfwSys;
  std::unique_ptr<vkt::GLFWWindow> window;
  std::unique_ptr<vkt::VkSys> vkSys;
  vkt::DebugMessengerBuilder debuggerBuilder;
  std::shared_ptr<vkt::Instance> instance;
  std::unique_ptr<vkt::DebugMessenger> debugger;
  std::unique_ptr<vkt::Surface> surf;
  std::vector<std::string> reqDevExts, customLayers, customExts;
  std::vector<vkt::PhysicalDevice> physDevs;
  std::optional<ChosenDev> chosenDev;
  std::shared_ptr<vkt::Device> dev;
  vkt::SwapchainBuilder swapchainBuilder;
  VkQueue graphicsQueue, presentQueue;
  VkViewport viewport;
  VkExtent2D extent;
  VkRect2D scissor;
  std::unique_ptr<vkt::Swapchain> swapchain;
  std::vector<vkt::ImageView> imageViews;
  std::shared_ptr<vkt::ShaderModule> vertShader, fragShader;
  std::unique_ptr<vkt::RenderPass> renderPass;
  std::unique_ptr<vkt::Pipeline> pipeline;
  std::vector<vkt::Framebuffer> framebuffers;
  std::unique_ptr<vkt::Semaphore> imageAvailSem, renderFinishedSem;
  std::unique_ptr<vkt::Fence> inFlightFence;
  std::unique_ptr<vkt::CommandPool> cmdPool;
  std::unique_ptr<vkt::CommandBuffer> cmdBuf;
  vkt::QueueSubmitOp queueSubmit;
  vkt::QueuePresentOp present;
};

int main() {
  return Main()();
}