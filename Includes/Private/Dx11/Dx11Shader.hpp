#pragma once
#include <Public/Shader.hpp>
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <Public/InputLayout.hpp>

#include <vector>
#include <string>

namespace mr {

class Dx11Shader : public Shader {
public:
	Dx11Shader(const std::wstring& name, InputLayout layout, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~Dx11Shader() = default;
	void bind() const final override;

private:
	Microsoft::WRL::ComPtr <ID3D11VertexShader> m_vertexShader  = nullptr;
	Microsoft::WRL::ComPtr <ID3D11PixelShader> m_pixelShader	= nullptr;

	Microsoft::WRL::ComPtr <ID3DBlob> m_vsBlob					= nullptr;
	Microsoft::WRL::ComPtr <ID3DBlob> m_psBlob					= nullptr;

	Microsoft::WRL::ComPtr <ID3D11InputLayout> m_inputLayout	= nullptr;

	ID3D11Device* p_device										= nullptr;
	ID3D11DeviceContext* p_context								= nullptr;
	std::vector <D3D11_INPUT_ELEMENT_DESC> m_layoutDesc			= {};
};

}