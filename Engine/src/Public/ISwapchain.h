#pragma once

#include "Types.h"

namespace MRe {

class ENGINE_API ISwapchain {
public:
    virtual auto Present(u32 idx)       -> void = 0;
    virtual auto GetNextImage()    -> u32 = 0;
};

}
