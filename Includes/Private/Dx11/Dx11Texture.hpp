#pragma once
#include <Public/Texture.hpp>
#include <string>

#include <d3d11.h>
#include <wrl/client.h>

namespace mr {

class Dx11Texture : public Texture {
public:
	Dx11Texture(const std::string& path, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~Dx11Texture() = default;
	void bind(int slot = 0) const final override;

private:
	std::string m_path													= "";
	Microsoft::WRL::ComPtr <ID3D11Texture2D> m_texture					= nullptr;
	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_resourceView	= nullptr;
	ID3D11Device* p_device												= nullptr;
	ID3D11DeviceContext* p_context										= nullptr;
};

}