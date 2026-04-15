#pragma once
#include <Public/VertexBuffer.hpp>
#include <wrl/client.h>
#include <d3d11.h>

namespace mr {

class Dx11VertexBuffer : public VertexBuffer {
public:
	Dx11VertexBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const void* data, int count, int stride, int offset = 0);
	~Dx11VertexBuffer() = default;
	void bind() const final override;

private:
	Microsoft::WRL::ComPtr <ID3D11Buffer>	m_buffer	= nullptr;
	ID3D11Device*							p_device	= nullptr;
	ID3D11DeviceContext*					p_context	= nullptr;
	UINT									m_offset	= 0;
	UINT									m_stride	= 0;
};

}