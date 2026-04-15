#include <Private/Dx11/Dx11RenderAPI.hpp>
#include <Private/Dx11/DxError.hpp>

namespace mr {

void Dx11RenderAPI::init(HWND handle) {
	p_handle = handle;
	createDeviceSwapChain();
	createRenderTargetView();
	createDepthStencilState();
	createDepthStencilView();
	createViewPort();
	createRasterizerState();
	createSamplerState();

}

void Dx11RenderAPI::clear() {
	RUN(m_context->ClearRenderTargetView(m_targetView.Get(), m_clearColor.data()), m_device);
	RUN(m_context->ClearDepthStencilView(m_stencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0), m_device);
}

void Dx11RenderAPI::swap() {
	RUN(m_swapchain->Present(1, 0), m_device);
}

void Dx11RenderAPI::drawIndexed(int count) {
	RUN(m_context->DrawIndexed(count, 0, 0), m_device);
}

void Dx11RenderAPI::setClearColor(float r, float g, float b, float a) {
	m_clearColor = { r, g, b, a };
}

void Dx11RenderAPI::preRenderSetup() {
	ID3D11RenderTargetView* rtvs[] = { m_targetView.Get() };

	RUN(m_context->OMSetRenderTargets(1, rtvs, m_stencilView.Get()), m_device);
	RUN(m_context->OMSetDepthStencilState(m_stencilState.Get(), 0), m_device);
	RUN(m_context->RSSetViewports(1, &m_viewPort), m_device);

	if (m_wireFrame) {
		RUN(m_context->RSSetState(m_rasterizerWireFrame.Get()), m_device);
	}
	else {
		RUN(m_context->RSSetState(m_rasterizerSolid.Get()), m_device);
	}

	RUN(m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST), m_device);
}

void Dx11RenderAPI::setWireFrameMode(bool cond) {
	m_wireFrame = cond;
}

ID3D11Device* Dx11RenderAPI::getDevice() const {
	return m_device.Get();
}

ID3D11DeviceContext* Dx11RenderAPI::getContext() const {
	return m_context.Get();
}

#pragma region Initializations

void Dx11RenderAPI::createDeviceSwapChain() {
	DXGI_SWAP_CHAIN_DESC sd					= {};
	sd.BufferDesc.Width						= 0;
	sd.BufferDesc.Height					= 0;
	sd.BufferDesc.RefreshRate.Numerator		= 0;
	sd.BufferDesc.RefreshRate.Denominator	= 0;
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	sd.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count						= 1;
	sd.SampleDesc.Quality					= 0;
	sd.BufferCount							= 1;
	sd.OutputWindow							= p_handle;
	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.Windowed								= true;
	sd.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;

	unsigned int debugFlags = 0;

#ifdef _DEBUG
	debugFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	HRUN(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		debugFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_swapchain,
		&m_device,
		nullptr,
		&m_context
	));
}

void Dx11RenderAPI::createRenderTargetView() {
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	HRUN(m_swapchain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)));

	D3D11_TEXTURE2D_DESC bbDesc;
	pBackBuffer->GetDesc(&bbDesc);

	m_renderWidth	= bbDesc.Width;
	m_renderHeight	= bbDesc.Height;
	HRUN(m_device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_targetView));
}

void Dx11RenderAPI::createViewPort() {
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.Width	= static_cast <float>(m_renderWidth);
	m_viewPort.Height	= static_cast <float>(m_renderHeight);
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
}

void Dx11RenderAPI::createDepthStencilState() {
	D3D11_DEPTH_STENCIL_DESC dsd	= {};
	dsd.DepthEnable					= TRUE;
	dsd.DepthWriteMask				= D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc					= D3D11_COMPARISON_LESS;
	dsd.StencilEnable				= FALSE;

	HRUN(m_device->CreateDepthStencilState(&dsd, &m_stencilState));
}

void Dx11RenderAPI::createDepthStencilView() {
	D3D11_TEXTURE2D_DESC td = {};
	td.Width				= m_renderWidth;
	td.Height				= m_renderHeight;
	td.MipLevels			= 1;
	td.ArraySize			= 1;
	td.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Quality	= 0;
	td.SampleDesc.Count		= 1;
	td.Usage				= D3D11_USAGE_DEFAULT;
	td.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags		= 0;
	td.MiscFlags			= 0;

	Microsoft::WRL::ComPtr <ID3D11Texture2D> depthTexture;
	HRUN(m_device->CreateTexture2D(&td, nullptr, &depthTexture));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd	= {};
	dsvd.Format							= td.Format;
	dsvd.ViewDimension					= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice				= 0;

	HRUN(m_device->CreateDepthStencilView(depthTexture.Get(), &dsvd, &m_stencilView));
}

void Dx11RenderAPI::createRasterizerState() {
	D3D11_RASTERIZER_DESC rd	= {};
	rd.DepthClipEnable			= true;
	rd.FrontCounterClockwise	= true;
	rd.FillMode					= D3D11_FILL_SOLID;
	rd.CullMode					= D3D11_CULL_BACK;

	HRUN(m_device->CreateRasterizerState(&rd, m_rasterizerSolid.GetAddressOf()));

	D3D11_RASTERIZER_DESC rdw	= {};
	rdw.DepthClipEnable			= true;
	rdw.FrontCounterClockwise	= true;
	rdw.FillMode				= D3D11_FILL_WIREFRAME;
	rdw.CullMode				= D3D11_CULL_BACK;

	HRUN(m_device->CreateRasterizerState(&rdw, m_rasterizerWireFrame.GetAddressOf()));
}

void Dx11RenderAPI::createSamplerState() {
	D3D11_SAMPLER_DESC sd	= {};
	sd.Filter				= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU				= D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV				= D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW				= D3D11_TEXTURE_ADDRESS_WRAP;

	HRUN(m_device->CreateSamplerState(&sd, m_samplerState.GetAddressOf()));
	RUN(m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf()), m_device);
}

#pragma endregion

}