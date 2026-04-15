#pragma once
#include "RenderAPI.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "GraphicsAPI.hpp"
#include "CoreAPI.hpp"
#include "Shader.hpp"
#include "InputLayout.hpp"
#include "ConstantBuffer.hpp"
#include "ShaderType.hpp"
#include "Texture.hpp"

#include <string>

namespace mr {

class RENDER_API GraphicsFactory {
public:
	virtual std::unique_ptr <RenderAPI> createRenderAPI(HWND handle)	= 0;
	virtual std::unique_ptr <VertexBuffer> createVertexBuffer
		(const void* data, int count, int stride, int offset = 0)		= 0;
	virtual std::unique_ptr <IndexBuffer> createIndexBuffer
		(const void* data, int count, int offset = 0)					= 0;
	virtual std::unique_ptr <Shader> createShader
		(const std::wstring& name, const InputLayout& layout)			= 0;
	virtual std::unique_ptr <ConstantBuffer> createConstantBuffer
		(const void* data, int size, ShaderType type, int slot = 0)		= 0;
	virtual std::unique_ptr <Texture> createTexture
		(const std::string& path)										= 0;

	static std::unique_ptr <GraphicsFactory> create(GraphicsAPI api);
};

}