#pragma once
#include <Public/GraphicsFactory.hpp>
#include <d3d11.h>

namespace mr {

class Dx11GraphicsFactory : public GraphicsFactory {
public:
	std::unique_ptr <RenderAPI> createRenderAPI(HWND handle)		final override;
	std::unique_ptr <VertexBuffer> createVertexBuffer
		(const void* data, int count, int stride, int offset = 0)	final override;
	std::unique_ptr <IndexBuffer> createIndexBuffer
		(const void* data, int count, int offset = 0)				final override;
	std::unique_ptr <Shader> createShader
		(const std::wstring& name, const InputLayout& layout)		final override;
	std::unique_ptr <ConstantBuffer> createConstantBuffer
		(const void* data, int size, ShaderType type, int slot = 0)	final override;
	std::unique_ptr <Texture> createTexture
		(const std::string& path)									final override;

private:
	ID3D11Device*			p_device			= nullptr;
	ID3D11DeviceContext*	p_context			= nullptr;
};

}