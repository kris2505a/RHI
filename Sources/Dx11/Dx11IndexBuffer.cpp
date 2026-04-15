#include <Private/Dx11/Dx11IndexBuffer.hpp>
#include <cstdint>
#include <Private/Dx11/DxError.hpp>

namespace mr {

Dx11IndexBuffer::Dx11IndexBuffer
	(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const void* indices, int count, int offset) 
	:	p_device(pDevice), p_context(pContext), 
		m_count(static_cast <UINT>(count)), m_offset(static_cast <UINT>(offset)) {

	D3D11_BUFFER_DESC bd	= {};
	bd.Usage				= D3D11_USAGE_DEFAULT;
	bd.BindFlags			= D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags		= 0;
	bd.MiscFlags			= 0;
	bd.ByteWidth			= count * sizeof(uint32_t);
	bd.StructureByteStride	= sizeof(uint32_t);

	D3D11_SUBRESOURCE_DATA srd	= {};
	srd.pSysMem					= indices;

	HRUN(p_device->CreateBuffer(&bd, &srd, &m_buffer));

}

void Dx11IndexBuffer::bind() const {
	RUN(p_context->IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R32_UINT, m_offset), p_device);
}

UINT Dx11IndexBuffer::getCount() const {
	return m_count;
}

}