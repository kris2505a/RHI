#include "VukSwapchain.h"

#include <print>

namespace MRe {

auto VukSwapchain::GetHandleMut() -> vk::raii::SwapchainKHR& {
    return m_Swapchain;
}

auto VukSwapchain::GetImagesMut() -> std::vector<vk::Image>& {
    return m_Images;
}

auto VukSwapchain::GetImageViewsMut() -> std::vector<vk::raii::ImageView>& {
    return m_ImageViews;
}

auto VukSwapchain::GetNextImage() -> u32 {
    //WILL BE MOVED TO DEVICE. NO USE HERE. FUCK OFF 
    return 0;
}

auto VukSwapchain::SetDeps(vk::raii::Queue* pPresentQueue, VukSyncData* pSyncData) -> void {
    p_PresentQueue = pPresentQueue;
    p_SyncData = pSyncData;
}


auto VukSwapchain::Present(u32 idx) -> void {
    std::array renderSemaphore {
        *p_SyncData->RenderFinished
    };
    std::array sc {
        *m_Swapchain
    };
    vk::PresentInfoKHR presentInfo {
        .waitSemaphoreCount = static_cast<u32>(renderSemaphore.size()),
        .pWaitSemaphores = renderSemaphore.data(),
        .swapchainCount = static_cast<u32>(sc.size()),
        .pSwapchains = sc.data(),
        .pImageIndices = &idx
    };

    auto res = p_PresentQueue->presentKHR(presentInfo);
    if (res == vk::Result::eErrorUnknown) {
        std::println("Unknown error on swap");
    }
}

}
