#include <Public/ConstantBuffer.hpp>
#include <Public/ShaderType.hpp>

#include <wrl/client.h>
#include <d3d11.h>

namespace mr {

class Dx11ConstantBuffer : public ConstantBuffer {
public:
	Dx11ConstantBuffer(
		const void* data,
		int size, ShaderType type,
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext,
		int slot = 0
	);
	~Dx11ConstantBuffer()					= default;
	void bind()								const final override;
	void update(const void* data, int size)	final override;

private:
	Microsoft::WRL::ComPtr <ID3D11Buffer> m_buffer	= nullptr;
	ShaderType m_type								= ShaderType::None;
	int m_slot										= 0;
	ID3D11Device* p_device							= nullptr;
	ID3D11DeviceContext* p_context					= nullptr;
};

}