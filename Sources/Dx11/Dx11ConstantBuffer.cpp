#include <Private/Dx11/Dx11ConstantBuffer.hpp>
#include <Private/Dx11/DxError.hpp>

namespace mr {

Dx11ConstantBuffer::Dx11ConstantBuffer(const void* data, int size, ShaderType type, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, int slot) {
	m_slot		= slot;
	m_type		= type;
	p_device	= pDevice;
	p_context	= pContext;

	D3D11_BUFFER_DESC bd	= {};
	bd.Usage				= D3D11_USAGE_DYNAMIC;
	bd.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags			= 0;
	bd.ByteWidth			= (size + 15) & ~15;
	bd.StructureByteStride	= 0;

	D3D11_SUBRESOURCE_DATA srd = {};
	srd.pSysMem = data;

	HRUN(p_device->CreateBuffer(&bd, &srd, &m_buffer));
}

void Dx11ConstantBuffer::bind() const {
	switch (m_type) {
	case ShaderType::VertexShader:
		RUN(p_context->VSSetConstantBuffers(
			static_cast <UINT>(m_slot),
			1,
			m_buffer.GetAddressOf()),
			p_device
		);
		break;

	case ShaderType::PixelShader:
		RUN(p_context->PSSetConstantBuffers(
			static_cast <UINT>(m_slot),
			1,
			m_buffer.GetAddressOf()),
			p_device
		);
		break;
	}
}

void Dx11ConstantBuffer::update(const void* data, int size) {
	auto roundedSize = (size + 15) & ~15;

	D3D11_MAPPED_SUBRESOURCE map;
	RUN(p_context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map), p_device);
	memcpy(map.pData, data, roundedSize);
	RUN(p_context->Unmap(m_buffer.Get(), 0), p_device);
}

}