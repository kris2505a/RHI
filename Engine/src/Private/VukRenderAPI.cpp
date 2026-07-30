#include "VukRenderAPI.h"
#include <print>
#define GLFW_EXPOSE_NATIVE_WAYLAND
#include <GLFW/glfw3native.h>

namespace MRe {

VukRenderAPI::VukRenderAPI(GLFWwindow* window) : p_Window(window) {
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateSyncObjects();
    CreateSwapChain();
    CreateImageViews();
    
    CreateCommandPoolAndBuffer();

}

auto VukRenderAPI::GetRequiredExtensions() -> std::vector<const char*> {

    uint32_t extensionCount;
    auto requiredExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

    std::vector<const char*> extensions(
        requiredExtensions, 
        requiredExtensions + extensionCount
    );

    if (g_Debug) {
        extensions.push_back(vk::EXTDebugUtilsExtensionName);
    }

    auto extensionProperties = m_Context.enumerateInstanceExtensionProperties();

    for (const char* i : extensions) {
        auto missing = std::none_of(
            extensionProperties.begin(), 
            extensionProperties.end(), 
            [i](const auto& extension) {
                return strcmp(extension.extensionName, i) == 0;
            }
        );

        if (missing) {
            throw std::runtime_error("Required Instance not found: " + std::string(i));
        }
    }

    return extensions;
}

auto VukRenderAPI::GetRequiredLayers() -> std::vector<const char*> {
    std::vector <const char*> layers;
    if (g_Debug) {
        layers.push_back("VK_LAYER_KHRONOS_validation");
    }

    auto layerProperties = m_Context.enumerateInstanceLayerProperties();

    for (const char* layer : layers) {
        auto missing = std::none_of(
            layerProperties.begin(),
            layerProperties.end(),
            [layer] (const vk::LayerProperties& property) {
                return strcmp(property.layerName, layer) == 0;
            }
        );

        if (missing) {
            throw std::runtime_error("Required layer not found: " + std::string(layer));
        }
    }

    return layers;
}

auto VukRenderAPI::CreateInstance() -> void {
    constexpr vk::ApplicationInfo appInfo {
        .pApplicationName = "VKEngine",
        .applicationVersion = vk::makeVersion(1, 0, 0),
        .pEngineName = "NoEngine",
        .engineVersion = vk::makeVersion(1, 0, 0),
        .apiVersion = vk::ApiVersion14
    };

    auto extensions = GetRequiredExtensions();
    auto layers = GetRequiredLayers();

    vk::InstanceCreateInfo createInfo {
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = static_cast<uint32_t>(layers.size()),
        .ppEnabledLayerNames = layers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data(),
    };

    m_Instance = vk::raii::Instance(m_Context, createInfo);
}

auto VukRenderAPI::SetupDebugMessenger() -> void {

    if (!g_Debug) {
        return;
    }

    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
    );

    vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags (
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
    );

    vk::DebugUtilsMessengerCreateInfoEXT createInfo {
        .messageSeverity = severityFlags,
        .messageType = messageTypeFlags,
        .pfnUserCallback = &VukRenderAPI::debugCallback
    };

    m_DebugMessenger = m_Instance.createDebugUtilsMessengerEXT(createInfo);
    
}

 VKAPI_ATTR VkBool32 VKAPI_CALL VukRenderAPI::debugCallback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
        vk::DebugUtilsMessageTypeFlagsEXT type,
        const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
) {
    
    std::println("Validation layer: [{}] : Type-> {} => Msg-> {}", vk::to_string(severity), vk::to_string(type), pCallbackData->pMessage);
    return VK_FALSE;
}

auto VukRenderAPI::IsDeviceSuitable(const vk::raii::PhysicalDevice& physicalDevice) -> bool {
    
    auto supportsVk13 = physicalDevice.getProperties().apiVersion >= vk::ApiVersion13;

    auto queueFamilies = physicalDevice.getQueueFamilyProperties();
    auto supportGraphics = std::any_of(
        queueFamilies.begin(),
        queueFamilies.end(),
        [](const vk::QueueFamilyProperties& queue) {
            return (queue.queueFlags & vk::QueueFlagBits::eGraphics) != vk::QueueFlagBits(0);
        }
    );

    std::vector<const char*> requiredDeviceExtension = {
        vk::KHRSwapchainExtensionName
    };
    auto availableDeviceExtensions = physicalDevice.enumerateDeviceExtensionProperties();
    auto supportExtensions = true;

    for (const char* extension : requiredDeviceExtension) {
        bool missing = std::none_of(
            availableDeviceExtensions.begin(),
            availableDeviceExtensions.end(),
            [extension](const vk::ExtensionProperties& extProps) {
                return strcmp(extension, extProps.extensionName) == 0;
            }
        );
        if (missing) {
            supportExtensions = false;
            break;
        }
    }

    auto features = physicalDevice.template getFeatures2<vk::PhysicalDeviceFeatures2,
                                                         vk::PhysicalDeviceVulkan11Features,
                                                         vk::PhysicalDeviceVulkan13Features,
                                                         vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();

    bool supportsRequiredFeatures = features.template get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters &&
                                    features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
                                    features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;

    return supportsVk13 && supportGraphics && supportExtensions && supportsRequiredFeatures;
}

auto VukRenderAPI::PickPhysicalDevice() -> void {
    std::vector<vk::raii::PhysicalDevice> physicalDevices = m_Instance.enumeratePhysicalDevices();

    const auto deviceIter = std::find_if(
        physicalDevices.begin(),
        physicalDevices.end(),
        [&](const auto& device) {
            return IsDeviceSuitable(device);
        }
    );

    if (deviceIter == physicalDevices.end()) {
        throw std::runtime_error("Suitable physical device not found");
    }
    
    m_PhysicalDevice = (*deviceIter);
}

auto VukRenderAPI::CreateLogicalDevice() -> void {
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = m_PhysicalDevice.getQueueFamilyProperties();
    auto graphicsQueueFamilyProperty = std::find_if(
        queueFamilyProperties.begin(), 
        queueFamilyProperties.end(), 
        [this](const auto& queueFamily) {
            return (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) != vk::QueueFlags(0);
        }
    );
    auto graphicsIndex = static_cast<uint32_t>(std::distance(queueFamilyProperties.begin(), graphicsQueueFamilyProperty));
    float queuePriority = 0.5f;

    vk::DeviceQueueCreateInfo deviceQueueCreateInfo {
        .queueFamilyIndex = graphicsIndex,
        .queueCount = 1,
        .pQueuePriorities = &queuePriority,
    };

    vk::PhysicalDeviceFeatures deviceFeatures;
    vk::StructureChain<vk::PhysicalDeviceFeatures2,
                   vk::PhysicalDeviceVulkan11Features,
                   vk::PhysicalDeviceVulkan13Features,
                   vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
    featureChain = {
        {},
        {.shaderDrawParameters = true},
        {.dynamicRendering = true},
        {.extendedDynamicState = true}
    };

    std::vector<const char*> requiredDeviceExtension = {
        vk::KHRSwapchainExtensionName
    };

    vk::DeviceCreateInfo deviceCreateInfo {
        .pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &deviceQueueCreateInfo,
        .enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtension.size()),
        .ppEnabledExtensionNames = requiredDeviceExtension.data(),
    };

    m_Device = vk::raii::Device(m_PhysicalDevice, deviceCreateInfo);
    m_GraphicsQueue = vk::raii::Queue(m_Device, graphicsIndex, 0);

    m_GraphicsQueueIndex = graphicsIndex;
}

auto VukRenderAPI::CreateSurface() -> void {
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(*m_Instance, p_Window, nullptr, &surface) != 0) {
        throw std::runtime_error("failed to create window surface");
    }

    m_Surface = vk::raii::SurfaceKHR(m_Instance, surface);
}

auto VukRenderAPI::CreateSwapChain() -> void {

    auto capabilities = m_PhysicalDevice.getSurfaceCapabilitiesKHR(*m_Surface);
    auto formats = m_PhysicalDevice.getSurfaceFormatsKHR(*m_Surface);
    auto presentModes = m_PhysicalDevice.getSurfacePresentModesKHR(*m_Surface);


    auto format = std::find_if(
        formats.begin(),
        formats.end(),
        [] (const vk::SurfaceFormatKHR& format) {
            return format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
        }
    );

    if (format == formats.end()) {
        m_SwapchainSurfaceFormat = formats.front();
    }
    else {
        m_SwapchainSurfaceFormat = *format;
    }

    vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo;

    auto _presentMode = std::find(
        presentModes.begin(),
        presentModes.end(),
        vk::PresentModeKHR::eMailbox
    );

    if (_presentMode != presentModes.end()) {
        presentMode = *_presentMode;
    }

    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        m_Swapchain.m_Extent = capabilities.currentExtent;
    }
    else {
        int w, h;
        glfwGetFramebufferSize(p_Window, &w, &h);
    
        m_Swapchain.m_Extent = {
            std::clamp(uint32_t(w), capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
            std::clamp(uint32_t(h), capabilities.minImageExtent.height, capabilities.maxImageExtent.height),
        };
    }
    uint32_t imageCount = capabilities.minImageCount + 1;

    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
        imageCount = capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR swapchainInfo {
        .surface = *m_Surface,
        .minImageCount = imageCount,
        .imageFormat = m_SwapchainSurfaceFormat.format,
        .imageColorSpace = m_SwapchainSurfaceFormat.colorSpace,
        .imageExtent = m_Swapchain.m_Extent,
        .imageArrayLayers = 1,
        .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
        .imageSharingMode = vk::SharingMode::eExclusive,
        .preTransform = capabilities.currentTransform,
        .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
        .presentMode = presentMode,
        .clipped = VK_TRUE,
    };

    m_Swapchain.GetHandleMut() = vk::raii::SwapchainKHR(m_Device, swapchainInfo);
    m_Swapchain.GetImagesMut() = m_Swapchain.m_Swapchain.getImages();
    m_Swapchain.SetDeps(&m_GraphicsQueue, &m_SyncData);
}

auto VukRenderAPI::CreateImageViews() -> void {
    vk::ImageViewCreateInfo imageViewCreateInfo {
        .viewType = vk::ImageViewType::e2D,
        .format = m_SwapchainSurfaceFormat.format,
        .subresourceRange = {
            vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1,
        }
    };

    imageViewCreateInfo.subresourceRange.levelCount = 1;
    imageViewCreateInfo.subresourceRange.layerCount = 1;

    imageViewCreateInfo.components = {
        vk::ComponentSwizzle::eIdentity,
        vk::ComponentSwizzle::eIdentity,
        vk::ComponentSwizzle::eIdentity,
        vk::ComponentSwizzle::eIdentity,
    };

    for (auto& image : m_Swapchain.m_Images) {
        imageViewCreateInfo.image = image;
        m_Swapchain.GetImageViewsMut().emplace_back(m_Device, imageViewCreateInfo);
    }
}

auto VukRenderAPI::CreateCommandPoolAndBuffer() -> void {
    vk::CommandPoolCreateInfo createInfo {
        .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
        .queueFamilyIndex = m_GraphicsQueueIndex
    };

    m_CommandPool = vk::raii::CommandPool(m_Device, createInfo);

    vk::CommandBufferAllocateInfo cmdBuffInfo {
        .commandPool = m_CommandPool,
        .level = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = 1
    };

    m_CommandBuffer = std::move(vk::raii::CommandBuffers(m_Device, cmdBuffInfo).front());

}

auto VukRenderAPI::CreateSyncObjects() -> void {
    vk::SemaphoreCreateInfo createInfo {};

    m_SyncData.ImageAvailable = vk::raii::Semaphore(m_Device, createInfo);
    m_SyncData.RenderFinished = vk::raii::Semaphore(m_Device, createInfo);
}

auto VukRenderAPI::GetSwapchain() -> ISwapchain& {
    return m_Swapchain;
}

}

