#include <Private/Dx11/Dx11VertexBuffer.hpp>
#include <Private/Dx11/DxError.hpp>

namespace mr {

Dx11VertexBuffer::Dx11VertexBuffer(
	ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const void* data, int count, int stride, int offset)
	: p_device(pDevice), p_context(pContext), m_stride(stride), m_offset(offset) {

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.ByteWidth = m_stride * count;
	bd.StructureByteStride = m_stride;

	D3D11_SUBRESOURCE_DATA srd = {};
	srd.pSysMem = data;

	HRUN(p_device->CreateBuffer(&bd, &srd, &m_buffer));

}

void Dx11VertexBuffer::bind() const {
	RUN(p_context->IASetVertexBuffers(0, 1, m_buffer.GetAddressOf(), &m_stride, &m_offset), p_device);
}
 
}