#pragma once

#include "Public/ISwapchain.h"

#include <vulkan/vulkan_raii.hpp>

#include "VukSync.h"

namespace MRe {

class VukSwapchain : public ISwapchain {
    friend class VukRenderAPI;
public:
    VukSwapchain() = default;
    ~VukSwapchain() = default;
    auto Present(u32 idx) -> void override;
    auto GetNextImage() -> u32 override;

private:
    auto GetHandleMut() -> vk::raii::SwapchainKHR&;
    auto GetImagesMut() -> std::vector<vk::Image>&;
    auto GetImageViewsMut() -> std::vector<vk::raii::ImageView>&;
    auto SetDeps(vk::raii::Queue* pQueue, VukSyncData* pSyncData) -> void;

private:
    vk::raii::SwapchainKHR              m_Swapchain { nullptr };
    std::vector<vk::Image>              m_Images;
    std::vector<vk::raii::ImageView>    m_ImageViews;
    vk::Extent2D                        m_Extent;

    vk::raii::Queue*                    p_PresentQueue;
    VukSyncData*                        p_SyncData;
};

}
