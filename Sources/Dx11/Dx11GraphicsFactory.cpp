#include <Private/Dx11/Dx11GraphicsFactory.hpp>
#include <Private/Dx11/Dx11RenderAPI.hpp>
#include <Private/Dx11/Dx11VertexBuffer.hpp>
#include <Private/Dx11/Dx11IndexBuffer.hpp>
#include <Private/Dx11/Dx11Shader.hpp>
#include <Private/Dx11/Dx11ConstantBuffer.hpp>
#include <Private/Dx11/Dx11Texture.hpp>

namespace mr {

std::unique_ptr<RenderAPI> Dx11GraphicsFactory::createRenderAPI(HWND handle) {
    auto api = std::make_unique <Dx11RenderAPI>();
    api->init(handle);
    p_device    = api->getDevice();
    p_context   = api->getContext();
    return api;
}

std::unique_ptr<VertexBuffer> Dx11GraphicsFactory::createVertexBuffer(const void* data, int count, int stride, int offset) {
    return std::make_unique<Dx11VertexBuffer>(p_device, p_context, data, count, stride, offset);
}

std::unique_ptr<IndexBuffer> Dx11GraphicsFactory::createIndexBuffer(const void* data, int count, int offset) {
    return std::make_unique<Dx11IndexBuffer>(p_device, p_context, data, count, offset);
}

std::unique_ptr<Shader> Dx11GraphicsFactory::createShader(const std::wstring& name, const InputLayout& layout) {
    return std::make_unique<Dx11Shader>(name, layout, p_device, p_context);
}

std::unique_ptr<ConstantBuffer> Dx11GraphicsFactory::createConstantBuffer(const void* data, int size, ShaderType type, int slot) {
    return std::make_unique<Dx11ConstantBuffer>(data, size, type, p_device, p_context, slot);
}

std::unique_ptr<Texture> Dx11GraphicsFactory::createTexture(const std::string& path) {
    return std::make_unique<Dx11Texture>(path, p_device, p_context);
}

}