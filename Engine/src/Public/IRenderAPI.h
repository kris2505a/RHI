#pragma once

#include "Types.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "ISwapchain.h"


namespace MRe {

class ENGINE_API IRenderAPI {
public:
    IRenderAPI() = default;
    virtual ~IRenderAPI() = default;
    static auto Create(GLFWwindow* window) -> Scope<IRenderAPI>;
    
    virtual auto GetSwapchain() -> ISwapchain& = 0;
    
};

}
