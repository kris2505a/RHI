#pragma once
#include "Public/IRenderAPI.h"

#include <vulkan/vulkan_raii.hpp>

#include "VukSwapchain.h"

namespace MRe {


#ifdef MI_DEBUG
constexpr bool g_Debug = true;
#else
constexpr bool g_Debug = false;
#endif

class VukRenderAPI : public IRenderAPI {
public:
    VukRenderAPI(GLFWwindow* window);
    ~VukRenderAPI() = default;

    auto GetSwapchain() -> ISwapchain& override;
    auto RenderFrameTemp(u32 idx) -> void override;
    
private:
    auto CreateInstance()               -> void;
    auto SetupDebugMessenger()          -> void;
    auto PickPhysicalDevice()           -> void;
    auto CreateLogicalDevice()          -> void;
    auto CreateSyncObjects()            -> void;
    auto CreateSurface()                -> void;
    auto CreateSwapChain()              -> void;
    auto CreateImageViews()             -> void;
    auto CreateCommandPoolAndBuffer()   -> void;

    auto GetRequiredExtensions()        -> std::vector<const char*>;
    auto GetRequiredLayers()            -> std::vector<const char*>;

    auto IsDeviceSuitable(const vk::raii::PhysicalDevice& physicalDevice) -> bool;
    
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
        vk::DebugUtilsMessageTypeFlagsEXT type,
        const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
        [[maybe_unused]]void* pUserData
    );

//Members
private:
    vk::raii::Context                   m_Context;
    vk::raii::Instance                  m_Instance          { nullptr };
    vk::raii::DebugUtilsMessengerEXT    m_DebugMessenger    { nullptr };
    vk::raii::PhysicalDevice            m_PhysicalDevice    { nullptr };
    vk::raii::Device                    m_Device            { nullptr };
    vk::raii::Queue                     m_GraphicsQueue     { nullptr };
    vk::raii::SurfaceKHR                m_Surface           { nullptr };

    vk::SurfaceFormatKHR                m_SwapchainSurfaceFormat;
    VukSwapchain                        m_Swapchain;

    vk::raii::CommandPool               m_CommandPool       { nullptr };
    vk::raii::CommandBuffer             m_Cmd     { nullptr };
    
    VukSyncData                         m_SyncData;

    u32 m_GraphicsQueueIndex { std::numeric_limits<u32>::max() };

//Pointer/Reference
private:
    GLFWwindow* p_Window;

};

}
