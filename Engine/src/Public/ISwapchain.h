#pragma once

#include "Types.h"

namespace MRe {

class ENGINE_API ISwapchain {
public:
    virtual auto Present(u32 idx)       -> void = 0;
    virtual auto GetNextImage()    -> u32 = 0;

    template<typename T>
    requires std::is_base_of_v<ISwapchain, T>
    auto As() -> T& {
        return dynamic_cast<T&> (*this);
    }

};

}
