#include "IRenderAPI.h"

#include "Private/VukRenderAPI.h"

namespace MRe {

auto IRenderAPI::Create(GLFWwindow* window) -> Scope<IRenderAPI> {
    return CreateScope<VukRenderAPI>(window);
}

}
