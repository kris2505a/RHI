#pragma once
#include <Public/RenderAPI.hpp>
#include <wrl/client.h>
#include <d3d11.h>

#include <array>

namespace mr {

class Dx11RenderAPI : public RenderAPI {
public:
	Dx11RenderAPI()		= default;
	~Dx11RenderAPI()	= default;

	void init(HWND handle)									final override;
	void clear()											final override;
	void swap()												final override;
	void drawIndexed(int count)								final override;
	void setClearColor(float r, float g, float b, float a)	final override;
	void preRenderSetup()									final override;
	void setWireFrameMode(bool cond)						final override;

	ID3D11Device* getDevice()										 const;
	ID3D11DeviceContext* getContext()								 const;


private:
	void createDeviceSwapChain();
	void createRenderTargetView();
	void createViewPort();
	void createDepthStencilState();
	void createDepthStencilView();
	void createRasterizerState();
	void createSamplerState();

private:
	HWND p_handle															= nullptr;
	Microsoft::WRL::ComPtr <ID3D11Device>			 m_device				= nullptr;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext>	 m_context				= nullptr;
	Microsoft::WRL::ComPtr <IDXGISwapChain>			 m_swapchain			= nullptr;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView>	 m_targetView			= nullptr;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView>	 m_stencilView			= nullptr;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilState> m_stencilState			= nullptr;
	Microsoft::WRL::ComPtr <ID3D11RasterizerState>	 m_rasterizerSolid		= nullptr;
	Microsoft::WRL::ComPtr <ID3D11RasterizerState>	 m_rasterizerWireFrame	= nullptr;
	Microsoft::WRL::ComPtr <ID3D11SamplerState>		 m_samplerState			= nullptr;
	D3D11_VIEWPORT									 m_viewPort				= {};
	std::array <float, 4>							 m_clearColor			= { 0.0f, 0.0f, 0.0f, 0.0f };
	UINT											 m_renderWidth			= 0;
	UINT											 m_renderHeight			= 0;
	bool											 m_wireFrame			= false;
};

}