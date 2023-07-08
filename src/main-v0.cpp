// #define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
// #include <vulkan/vulkan.hpp>
// #define GLFW_INCLUDE_NONE
// #include <GLFW/glfw3.h>
// #include <cstdlib>
// #include <iostream>
// #include <memory>
// #include <optional>
// #include <stdexcept>
// #include <string>
// #include <type_traits>
// #include <unordered_map>
// #include <vector>

// class glfw_system {
// public:
//   glfw_system() {
//     if (glfwInit() != GLFW_TRUE)
//       throw std::runtime_error("glfwInit");
//   }

//   std::vector<std::string> get_vk_instance_extensions() {
//     uint32_t num_ext;
//     auto **glfw_ext = glfwGetRequiredInstanceExtensions(&num_ext);
//     std::vector<std::string> glfw_exts(glfw_ext, glfw_ext + num_ext);
//     return glfw_exts;
//   }

//   ~glfw_system() {
//     glfwTerminate();
//   }
// };

// template <typename Ret, typename... Args>
// class virt_cb {
// public:
//   virtual Ret impl(Args... args) = 0;
// };

// template <typename F, typename Ret, typename... Args>
// class lambda_cb : public virt_cb<Ret, Args...> {
// public:
//   explicit lambda_cb(F f) : f{std::move(f)} {}

//   virtual Ret impl(Args... args) {
//     if constexpr (std::is_void_v<Ret>)
//       f(std::forward<Args>(args)...);
//     else
//       return f(std::forward<Args>(args)...);
//   }

// private:
//   F f;
// };

// template <typename Ret, typename... Args>
// class any_cb : public virt_cb<Ret, Args...> {
// public:
//   virtual Ret impl(Args... args) {
//     static_assert(std::is_default_constructible_v<Ret> ||
//     std::is_void_v<Ret>,
//                   "Return type must be either void or
//                   default-constructible");
//     if (cb) {
//       if constexpr (std::is_void_v<Ret>)
//         cb->impl(std::forward<Args>(args)...);
//       else
//         return cb->impl(std::forward<Args>(args)...);
//     } else {
//       if constexpr (!std::is_void_v<Ret>) {
//         Ret v{};
//         return v;
//       }
//     }
//   }

//   Ret operator()(Args... args) {
//     if constexpr (std::is_void_v<Ret>)
//       impl(std::forward<Args>(args)...);
//     else
//       return impl(std::forward<Args>(args)...);
//   }

//   template <typename F>
//   any_cb &operator=(F f) {
//     cb = std::make_shared<lambda_cb<F, Ret, Args...>>(std::move(f));
//     return *this;
//   }

// private:
//   std::shared_ptr<virt_cb<Ret, Args...>> cb;
// };

// template <typename Sig>
// struct _cb_for_sig_def;

// template <typename Ret, typename... Args>
// struct _cb_for_sig_def<Ret (*)(Args...)> {
//   using type = any_cb<Ret, Args...>;
// };

// template <typename Sig>
// using cb_for_sig = typename _cb_for_sig_def<Sig>::type;

// template <typename Sig>
// struct _glfw_cb_def;

// template <typename... Args>
// struct _glfw_cb_def<void (*)(GLFWwindow *, Args...)> {
//   using type = any_cb<void, Args...>;
// };

// template <typename Sig>
// using glfw_cb = typename _glfw_cb_def<Sig>::type;

// enum glfw_cb_tag {
//   KEY_CB = 0,
//   WINDOW_SIZE_CB = 1,
// };

// struct glfw_cb_set {
//   glfw_cb<GLFWkeyfun> key_cb;
//   glfw_cb<GLFWwindowsizefun> window_size_cb;

//   template <int Tag>
//   decltype(auto) get() {
//     return std::get<Tag>(std::tie(key_cb, window_size_cb));
//   }

//   static void setup(GLFWwindow *window) {
//     glfwSetKeyCallback(window, _cb<KEY_CB, GLFWkeyfun>::impl);
//     glfwSetWindowSizeCallback(window,
//                               _cb<WINDOW_SIZE_CB, GLFWwindowsizefun>::impl);
//   }

//   template <int Tag, typename Sig>
//   struct _cb;

//   template <int Tag, typename... Args>
//   struct _cb<Tag, void (*)(GLFWwindow *, Args...)> {
//     static void impl(GLFWwindow *window, Args... args) {
//       auto *user_ptr = glfwGetWindowUserPointer(window);
//       auto *callbacks = reinterpret_cast<glfw_cb_set *>(user_ptr);
//       callbacks->get<Tag>().impl(std::forward<Args>(args)...);
//     }
//   };
// };

// class glfw_window {
// public:
//   glfw_window(glfw_system const &, int width, int height,
//               std::string const &title) {
//     ptr = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
//     if (!ptr)
//       throw std::runtime_error("glfwCreateWindow");

//     callbacks = std::make_shared<glfw_cb_set>();
//     glfwSetWindowUserPointer(ptr, callbacks.get());
//     glfw_cb_set::setup(ptr);
//   }

//   glfw_window(glfw_window const &) = delete;
//   glfw_window &operator=(glfw_window const &) = delete;

//   ~glfw_window() {
//     glfwDestroyWindow(ptr);
//   }

//   operator GLFWwindow *() {
//     return ptr;
//   }

//   operator GLFWwindow const *() const {
//     return ptr;
//   }

//   std::shared_ptr<glfw_cb_set> callbacks;

// private:
//   GLFWwindow *ptr;
// };

// class vk_system {
// public:
//   static std::vector<VkLayerProperties> get_layer_props() {
//     uint32_t prop_count;
//     vkEnumerateInstanceLayerProperties(&prop_count, nullptr);

//     std::vector<VkLayerProperties> layer_props(prop_count);
//     vkEnumerateInstanceLayerProperties(&prop_count, layer_props.data());

//     return layer_props;
//   }
// };

// class vk_avail_layers {
// public:
//   vk_avail_layers() {
//     uint32_t layer_count = 0;
//     vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

//     avail_layers = std::vector<VkLayerProperties>(layer_count);
//     vkEnumerateInstanceLayerProperties(&layer_count, avail_layers.data());
//   }

//   bool is_supported(std::string const &layer_name) {
//     for (auto const &layer : avail_layers) {
//       if (layer.layerName == layer_name)
//         return true;
//     }
//     return false;
//   }

// private:
//   std::vector<VkLayerProperties> avail_layers;
// };

// class vk_avail_exts {
// public:
//   vk_avail_exts() {
//     uint32_t ext_count;
//     vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, nullptr);

//     avail_exts = std::vector<VkExtensionProperties>(ext_count);
//     vkEnumerateInstanceExtensionProperties(nullptr, &ext_count,
//                                            avail_exts.data());
//   }

//   bool is_supported(std::string const &ext_name) {
//     for (auto const &ext : avail_exts) {
//       if (ext.extensionName == ext_name)
//         return true;
//     }
//     return false;
//   }

// private:
//   std::vector<VkExtensionProperties> avail_exts;
// };

// template <typename Data>
// class vk_wrapper {
// public:
//   operator Data &() {
//     return data;
//   }

//   operator Data const &() const {
//     return data;
//   }

//   Data *operator->() {
//     return &data;
//   }

//   Data const *operator->() const {
//     return &data;
//   }

// protected:
//   Data data = {};
// };

// template <typename CreateInfo>
// class vk_builder {
// public:
//   CreateInfo &get_create_info() {
//     return data;
//   }

//   CreateInfo const &get_create_info() const {
//     return data;
//   }

//   CreateInfo *operator->() {
//     return &data;
//   }

//   CreateInfo const *operator->() const {
//     return &data;
//   }

// protected:
//   CreateInfo data = {};
// };

// class vk_app_info : public vk_wrapper<VkApplicationInfo> {
// public:
//   vk_app_info() {
//     data.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//     data.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//     data.pEngineName = "N/A";
//     data.engineVersion = VK_MAKE_VERSION(1, 0, 0);
//     data.apiVersion = VK_API_VERSION_1_0;
//     // auto result = vkEnumerateInstanceVersion(&data.apiVersion);
//     // if (result != VK_SUCCESS)
//     //   throw std::runtime_error("vkEnumerateInstanceVersion");
//   }

//   vk_app_info(VkApplicationInfo const &app_info) {
//     this->data = app_info;
//     set_app_name(app_info.pApplicationName);
//   }

//   vk_app_info(vk_app_info const &other) {
//     *this = other;
//   }

//   vk_app_info &operator=(vk_app_info const &other) {
//     data = other.data;
//     set_app_name(other.app_name);
//     return *this;
//   }

//   vk_app_info(vk_app_info &&other) {
//     *this = std::move(other);
//   }

//   vk_app_info &operator=(vk_app_info &other) {
//     data = std::move(other.data);
//     other.app_name = std::move(other.app_name);
//     return *this;
//   }

//   vk_app_info &set_app_name(std::string const &app_name) {
//     this->app_name = app_name;
//     data.pApplicationName = this->app_name.c_str();
//     return *this;
//   }

// private:
//   std::string app_name;
// };

// class vk_instance : public vk_wrapper<VkInstance> {
// public:
//   explicit vk_instance(VkInstance &&instance) {
//     data = std::move(instance);
//   }

//   ~vk_instance() {
//     vkDestroyInstance(data, nullptr);
//   }

//   vk_instance(vk_instance const &) = delete;
//   vk_instance &operator=(vk_instance const &) = delete;

//   vk_instance(vk_instance &&) = default;
//   vk_instance &operator=(vk_instance &&) = default;

//   std::vector<VkPhysicalDevice> get_physical_devices() const {
//     uint32_t dev_count = 0;
//     auto res = vkEnumeratePhysicalDevices(data, &dev_count, VK_NULL_HANDLE);
//     if (res != VK_SUCCESS)
//       throw std::runtime_error("vkEnumeratePhysicalDevices");

//     if (dev_count == 0) {
//       return {};
//     } else {
//       std::vector<VkPhysicalDevice> devices(dev_count);
//       vkEnumeratePhysicalDevices(data, &dev_count, devices.data());
//       return devices;
//     }
//   }
// };

// class vk_phys_dev : public vk_wrapper<VkPhysicalDevice> {
// public:
//   vk_phys_dev(VkPhysicalDevice device) {
//     data = std::move(device);
//     vkGetPhysicalDeviceProperties(data, &props);
//     vkGetPhysicalDeviceFeatures(data, &features);

//     uint32_t qf_count = 0;
//     vkGetPhysicalDeviceQueueFamilyProperties(data, &qf_count,
//     VK_NULL_HANDLE); qf_props.resize(qf_count);
//     vkGetPhysicalDeviceQueueFamilyProperties(data, &qf_count,
//     qf_props.data());
//   }

//   VkPhysicalDeviceProperties props;
//   VkPhysicalDeviceFeatures features;
//   std::vector<VkQueueFamilyProperties> qf_props;
// };

// class vk_device_queue_create_info_ref {
// public:
//   vk_device_queue_create_info_ref(VkDeviceQueueCreateInfo &ref) : ref{ref} {}

//   void reset() {
//     ref = {};
//     ref.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//     set_count(1);
//   }

//   vk_device_queue_create_info_ref &set_index(uint32_t qf_index) {
//     ref.queueFamilyIndex = qf_index;
//     return *this;
//   }

//   vk_device_queue_create_info_ref &set_count(uint32_t count) {
//     ref.queueCount = count;
//     priorities = std::vector<float>(count, 1.0f);
//     ref.pQueuePriorities = priorities.data();
//     return *this;
//   }

//   std::vector<float> priorities;

// private:
//   VkDeviceQueueCreateInfo &ref;
// };

// class vk_device : public vk_wrapper<VkDevice> {
// public:
//   vk_device(VkDevice &&dev) {
//     data = std::move(dev);
//   }

//   ~vk_device() {
//     vkDestroyDevice(data, VK_NULL_HANDLE);
//   }

//   VkQueue get_queue(uint32_t qf_idx, uint32_t queue_idx) {
//     VkQueue queue;
//     vkGetDeviceQueue(data, qf_idx, queue_idx, &queue);
//     return queue;
//   }
// };

// class vk_device_builder : public vk_builder<VkDeviceCreateInfo> {
// public:
//   vk_device_builder() {
//     data.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//   }

//   vk_device_builder(vk_device_builder const &) = delete;
//   vk_device_builder &operator=(vk_device_builder const &) = delete;

//   vk_device_builder(vk_device_builder &&) = default;
//   vk_device_builder &operator=(vk_device_builder &&) = default;

//   vk_device_builder &set_queue_count(uint32_t count) {
//     data.queueCreateInfoCount = count;
//     queue_create_infos = std::vector<VkDeviceQueueCreateInfo>(count);
//     data.pQueueCreateInfos = queue_create_infos.data();
//     for (uint32_t idx = 0; idx < count; ++idx)
//       queue_create_info(idx).reset();
//     return *this;
//   }

//   vk_device_queue_create_info_ref queue_create_info(uint32_t idx) {
//     return vk_device_queue_create_info_ref(queue_create_infos[idx]);
//   }

//   vk_device_builder &
//   set_features(std::vector<VkPhysicalDeviceFeatures> const &features) {
//     this->features = features;
//     data.pEnabledFeatures = this->features.data();
//     return *this;
//   }

//   vk_device_builder &set_layers(std::vector<std::string> const &layers) {
//     this->layers = layers;
//     layer_ptrs.resize(layers.size());
//     for (size_t idx = 0; idx < layers.size(); ++idx)
//       layer_ptrs[idx] = layers[idx].c_str();
//     data.enabledLayerCount = (uint32_t)layers.size();
//     data.ppEnabledLayerNames = layer_ptrs.data();
//     return *this;
//   }

//   vk_device_builder &set_extensions(std::vector<std::string> const &exts) {
//     this->exts = exts;
//     ext_ptrs.resize(exts.size());
//     for (size_t idx = 0; idx < exts.size(); ++idx)
//       ext_ptrs[idx] = exts[idx].c_str();
//     data.enabledExtensionCount = (uint32_t)exts.size();
//     data.ppEnabledExtensionNames = ext_ptrs.data();
//     return *this;
//   }

//   vk_device build(VkPhysicalDevice const &phys_dev) const {
//     VkDevice dev;
//     if (vkCreateDevice(phys_dev, &data, VK_NULL_HANDLE, &dev) != VK_SUCCESS)
//       throw std::runtime_error("vkCreateDevice");
//     return vk_device(std::move(dev));
//   }

// private:
//   std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
//   std::vector<VkPhysicalDeviceFeatures> features;
//   std::vector<std::string> layers;
//   std::vector<char const *> layer_ptrs;
//   std::vector<std::string> exts;
//   std::vector<char const *> ext_ptrs;
// };

// class vk_instance_builder : public vk_builder<VkInstanceCreateInfo> {
// public:
//   vk_instance_builder() {
//     data.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//   }

//   void load(VkInstanceCreateInfo const &create_info) {
//     this->data = create_info;
//     set_app_info(*create_info.pApplicationInfo);

//     layers = std::vector<std::string>(create_info.enabledLayerCount);
//     for (size_t layer_idx = 0; layer_idx < layers.size(); ++layer_idx)
//       layers[layer_idx] = create_info.ppEnabledLayerNames[layer_idx];
//     set_layers(layers);

//     exts = std::vector<std::string>(create_info.enabledExtensionCount);
//     for (size_t ext_idx = 0; ext_idx < exts.size(); ++ext_idx)
//       exts[ext_idx] = create_info.ppEnabledExtensionNames[ext_idx];
//     set_extensions(exts);
//   }

//   vk_instance_builder(vk_instance_builder &other) {
//     *this = other;
//   }

//   vk_instance_builder &operator=(vk_instance_builder const &other) {
//     data = other.data;
//     set_app_info(other.app_info);
//     set_layers(other.layers);
//     set_extensions(other.exts);
//     return *this;
//   }

//   vk_instance_builder &set_app_info(vk_app_info const &app_info) {
//     this->app_info = app_info;
//     data.pApplicationInfo = &(VkApplicationInfo const &)this->app_info;
//     return *this;
//   }

//   vk_instance_builder &set_flags(VkInstanceCreateFlags const &flags) {
//     data.flags = flags;
//     return *this;
//   }

//   vk_instance_builder &set_layers(std::vector<std::string> const &layers) {
//     this->layers = layers;

//     layer_ptrs = std::vector<char const *>(layers.size());
//     for (size_t layer_idx = 0; layer_idx < layers.size(); ++layer_idx) {
//       layer_ptrs[layer_idx] = layers[layer_idx].c_str();
//     }

//     data.enabledLayerCount = (uint32_t)layers.size();
//     data.ppEnabledLayerNames = layer_ptrs.data();

//     return *this;
//   }

//   vk_instance_builder &
//   set_extensions(std::vector<std::string> const &extensions) {
//     this->exts = extensions;

//     ext_ptrs = std::vector<char const *>(exts.size());
//     for (size_t ext_idx = 0; ext_idx < exts.size(); ++ext_idx) {
//       ext_ptrs[ext_idx] = exts[ext_idx].c_str();
//     }

//     data.enabledExtensionCount = (uint32_t)exts.size();
//     data.ppEnabledExtensionNames = ext_ptrs.data();

//     return *this;
//   }

//   vk_instance build() const {
//     VkInstance instance;
//     auto result = vkCreateInstance(&data, nullptr, &instance);
//     if (result != VK_SUCCESS)
//       throw std::runtime_error("vkCreateInstance");
//     return vk_instance(std::move(instance));
//   }

// private:
//   vk_app_info app_info;

//   std::vector<std::string> layers;
//   std::vector<char const *> layer_ptrs;

//   std::vector<std::string> exts;
//   std::vector<char const *> ext_ptrs;
// };

// class vk_debug_utils_messenger : public vk_wrapper<VkDebugUtilsMessengerEXT>
// { public:
//   vk_debug_utils_messenger(VkDebugUtilsMessengerEXT &&debug_msgr,
//                            std::shared_ptr<vk_instance> instance) {
//     data = std::move(debug_msgr);
//     this->instance = std::move(instance);
//   }

//   vk_debug_utils_messenger(vk_debug_utils_messenger const &) = delete;
//   vk_debug_utils_messenger &
//   operator=(vk_debug_utils_messenger const &) = delete;

//   ~vk_debug_utils_messenger() {
//     vkDestroyDebugUtilsMessengerEXT(*instance, data, nullptr);
//   }

// private:
//   std::shared_ptr<vk_instance> instance;
// };

// class vk_debug_utils_messenger_builder
//     : public vk_builder<VkDebugUtilsMessengerCreateInfoEXT> {
// public:
//   vk_debug_utils_messenger_builder() {
//     data = {};
//     data.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
//     data.pUserData = this;
//     data.pfnUserCallback = _debug_cb;
//   }

//   vk_debug_utils_messenger_builder(
//       vk_debug_utils_messenger_builder const &other) {
//     data = {};
//     *this = other;
//   }

//   vk_debug_utils_messenger_builder &
//   operator=(vk_debug_utils_messenger_builder const &other) {
//     data = other.data;
//     data.pUserData = this;
//     debug_cb = other.debug_cb;
//     return *this;
//   }

//   vk_debug_utils_messenger_builder(vk_debug_utils_messenger_builder &&other)
//   {
//     data = {};
//     *this = std::move(other);
//   }

//   vk_debug_utils_messenger_builder &
//   operator=(vk_debug_utils_messenger_builder &&other) {
//     data = std::move(other.data);
//     data.pUserData = this;
//     debug_cb = std::move(other.debug_cb);
//     return *this;
//   }

//   vk_debug_utils_messenger_builder &
//   set_severity(VkDebugUtilsMessageSeverityFlagsEXT severity) {
//     data.messageSeverity = severity;
//     return *this;
//   }

//   vk_debug_utils_messenger_builder &
//   set_message_type(VkDebugUtilsMessageTypeFlagsEXT type) {
//     data.messageType = type;
//     return *this;
//   }

//   vk_debug_utils_messenger build(std::shared_ptr<vk_instance> instance) const
//   {
//     VkDebugUtilsMessengerEXT debug_msgr;
//     auto result =
//         vkCreateDebugUtilsMessengerEXT(*instance, &data, nullptr,
//         &debug_msgr);
//     if (result != VK_SUCCESS)
//       throw std::runtime_error("vkCreateDebugUtilsMessengerEXT");

//     return vk_debug_utils_messenger(std::move(debug_msgr),
//     std::move(instance));
//   }

//   using debug_cb_sig = VkBool32 (*)(
//       VkDebugUtilsMessageSeverityFlagBitsEXT,
//       VkDebugUtilsMessageTypeFlagsEXT, const
//       VkDebugUtilsMessengerCallbackDataEXT *);
//   cb_for_sig<debug_cb_sig> debug_cb;

// private:
//   static VKAPI_ATTR VkBool32 VKAPI_CALL _debug_cb(
//       VkDebugUtilsMessageSeverityFlagBitsEXT severity,
//       VkDebugUtilsMessageTypeFlagsEXT type,
//       const VkDebugUtilsMessengerCallbackDataEXT *cb_data, void *user_data) {
//     auto *messenger =
//         reinterpret_cast<vk_debug_utils_messenger_builder *>(user_data);
//     return messenger->debug_cb(severity, type, cb_data);
//   }
// };

// VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

// int main() {
//   glfw_system glfw_sys;

//   glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//   glfw_window window(glfw_sys, 1280, 960, "vktest");

//   window.callbacks->key_cb = [&](int key, int scancode, int action, int code)
//   {
//     if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
//       glfwSetWindowShouldClose(window, GLFW_TRUE);
//   };

//   VULKAN_HPP_DEFAULT_DISPATCHER.init(glfwGetInstanceProcAddress);

//   vk_app_info app_info;
//   app_info.set_app_name("vktest");

//   vk_instance_builder instance_builder;
//   instance_builder.set_flags(instance_builder->flags |
//                              VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR);
//   instance_builder.set_app_info(app_info);

//   std::vector<std::string> enabled_layers = {"VK_LAYER_KHRONOS_validation"};

//   vk_avail_layers avail_layers;
//   for (auto const &layer : enabled_layers) {
//     if (!avail_layers.is_supported(layer))
//       throw std::runtime_error("layer not available");
//   }

//   instance_builder.set_layers(enabled_layers);

//   std::vector<std::string> enabled_extensions = {
//       VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
//       VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME};

//   auto glfw_extensions = glfw_sys.get_vk_instance_extensions();
//   enabled_extensions.insert(enabled_extensions.end(),
//   glfw_extensions.begin(),
//                             glfw_extensions.end());

//   vk_avail_exts avail_exts;
//   for (auto const &ext : enabled_extensions) {
//     if (!avail_exts.is_supported(ext))
//       throw std::runtime_error("ext not available");
//   }

//   instance_builder.set_extensions(enabled_extensions);

//   vk_debug_utils_messenger_builder create_debug_builder;
//   create_debug_builder.set_severity(
//       VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
//       VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
//       VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);
//   create_debug_builder.set_message_type(
//       VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
//       VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
//       VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT);

//   create_debug_builder.debug_cb =
//       [&](VkDebugUtilsMessageSeverityFlagBitsEXT severity,
//           VkDebugUtilsMessageTypeFlagsEXT type,
//           const VkDebugUtilsMessengerCallbackDataEXT *cb_data) -> VkBool32 {
//     std::string severity_s;
//     switch (severity) {
//     case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
//       severity_s = "V";
//       break;
//     case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
//       severity_s = "W";
//       break;
//     case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
//       severity_s = "E";
//       break;
//     default:
//       severity_s = "?";
//       break;
//     };

//     std::cout << "[validation layer A] [" << severity_s << "] "
//               << cb_data->pMessage << std::endl;
//     return VK_FALSE;
//   };

//   instance_builder->pNext = &create_debug_builder.get_create_info();

//   auto instance = std::make_shared<vk_instance>(instance_builder.build());

//   VULKAN_HPP_DEFAULT_DISPATCHER.init((VkInstance)*instance);

//   auto phys_devs = instance->get_physical_devices();

//   while (!glfwWindowShouldClose(window)) {
//     glfwPollEvents();
//   }

//   return EXIT_SUCCESS;
// }