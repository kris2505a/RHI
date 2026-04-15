#include <Private/Dx11/Dx11Texture.hpp>
#include <Private/stb_image.h>
#include <Private/Dx11/DxError.hpp>

namespace mr {



Dx11Texture::Dx11Texture(const std::string& path, ID3D11Device* pDevice, ID3D11DeviceContext* pContext) {
	m_path		= path;
	p_device	= pDevice;
	p_context	= pContext;

	int width, height, bpp;	
	unsigned char* imgData = stbi_load(m_path.c_str(), &width, &height, &bpp, STBI_rgb_alpha);

	if (!imgData) {
		std::cerr << stbi_failure_reason() << std::endl;
		throw std::runtime_error(stbi_failure_reason());
	}

	D3D11_TEXTURE2D_DESC td = {};
	td.Width				= width;
	td.Height				= height;
	td.MipLevels			= 1;
	td.ArraySize			= 1;
	td.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count		= 1;
	td.Usage				= D3D11_USAGE_IMMUTABLE;
	td.BindFlags			= D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA srd{};
	srd.pSysMem = imgData;
	srd.SysMemPitch = width * 4;

	HRUN(p_device->CreateTexture2D(&td, &srd, m_texture.GetAddressOf()));
	HRUN(p_device->CreateShaderResourceView(
		m_texture.Get(),
		nullptr,
		m_resourceView.GetAddressOf()
	));
	if (imgData) {
		stbi_image_free(imgData);
	}
}

void Dx11Texture::bind(int slot) const {
	RUN(p_context->PSSetShaderResources(slot, 1, m_resourceView.GetAddressOf()), p_device);
}

}