#include <GLFW/glfw3.h>

#include <Public/IRenderAPI.h>

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    uint32_t width = 1280, height = 720;

    auto window = glfwCreateWindow(width, height, "VkWindow", nullptr, nullptr);

    auto rhi = MRe::IRenderAPI::Create(window);

    auto& swapchain = rhi->GetSwapchain();

    

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        auto idx = swapchain.GetNextImage();

        rhi->RenderFrameTemp(idx);

        swapchain.Present(idx);

    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
