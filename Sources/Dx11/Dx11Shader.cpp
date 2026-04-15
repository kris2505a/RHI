#include <Private/Dx11/Dx11Shader.hpp>
#include <Private/Dx11/DxError.hpp>


namespace mr {

Dx11Shader::Dx11Shader(const std::wstring& name, InputLayout layout, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: p_device(pDevice), p_context(pContext) {

	// Creation of shader
	std::wstring vsPath = L"shader/" + name + L"VS.cso";
	std::wstring psPath = L"shader/" + name + L"PS.cso";

	HRUN(D3DReadFileToBlob(vsPath.c_str(), m_vsBlob.GetAddressOf()));
	HRUN(D3DReadFileToBlob(psPath.c_str(), m_psBlob.GetAddressOf()));

	HRUN(p_device->CreateVertexShader(
		m_vsBlob->GetBufferPointer(),
		m_vsBlob->GetBufferSize(),
		nullptr,
		m_vertexShader.GetAddressOf()
	));
	HRUN(p_device->CreatePixelShader(
		m_psBlob->GetBufferPointer(),
		m_psBlob->GetBufferSize(),
		nullptr,
		m_pixelShader.GetAddressOf()
	));

	// Creation of input layout
	for (uint32_t i = 0; i < layout.getLayouts().size(); i++) {
		DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
		const auto& l = layout.getLayouts()[i];

		if (l.type == LayoutDataType::Float) {
			switch (l.count) {
			case 1:
				format = DXGI_FORMAT_R32_FLOAT;
				break;

			case 2:
				format = DXGI_FORMAT_R32G32_FLOAT;
				break;

			case 3:
				format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;

			case 4:
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			}
		}

		m_layoutDesc.push_back(
			{
				l.semanticName.c_str(),
				0,
				format,
				0,
				i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			}
		);
	}

	HRUN(p_device->CreateInputLayout(
		m_layoutDesc.data(),
		m_layoutDesc.size(),
		m_vsBlob->GetBufferPointer(),
		m_vsBlob->GetBufferSize(),
		m_inputLayout.GetAddressOf()
	));

}

void Dx11Shader::bind() const {
	RUN(p_context->VSSetShader(m_vertexShader.Get(), nullptr, 0), p_device);
	RUN(p_context->PSSetShader(m_pixelShader.Get(), nullptr, 0), p_device);
	RUN(p_context->IASetInputLayout(m_inputLayout.Get()), p_device);
}

}