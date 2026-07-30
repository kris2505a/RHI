#pragma once
#include "vulkan/vulkan_raii.hpp"

namespace MRe {

struct VukSyncData {
    vk::raii::Semaphore RenderFinished  { nullptr };
    vk::raii::Semaphore ImageAvailable  { nullptr };
    vk::raii::Fence     InFlight        { nullptr };
};

}
