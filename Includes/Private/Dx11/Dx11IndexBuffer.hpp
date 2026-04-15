#pragma once
#include <Public/IndexBuffer.hpp>
#include <d3d11.h>
#include <wrl/client.h>

namespace mr {

class Dx11IndexBuffer : public IndexBuffer {
public:
	Dx11IndexBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const void* indices, int count, int offset = 0);
	~Dx11IndexBuffer() = default;
	void bind()				const final override;
	unsigned int getCount() const final override;

private:
	Microsoft::WRL::ComPtr <ID3D11Buffer>	m_buffer	= nullptr;
	ID3D11Device*							p_device	= nullptr;
	ID3D11DeviceContext*					p_context	= nullptr;
	UINT									m_count		= 0;
	UINT									m_offset	= 0;
};

}