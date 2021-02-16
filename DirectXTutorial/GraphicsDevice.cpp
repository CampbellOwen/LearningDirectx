#include "GraphicsDevice.h"
#include "RenderTexture.h"

#include "Utils.h"

namespace Engine
{

	GraphicsDevice::GraphicsDevice(HWND hWnd, uint32_t width, uint32_t height)
	{
		DXGI_SWAP_CHAIN_DESC scd;

		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferCount = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.Width = width;
		scd.BufferDesc.Height = height;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = hWnd;
		scd.SampleDesc.Count = 4;
		scd.Windowed = TRUE;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			D3D11_CREATE_DEVICE_DEBUG,
			NULL,
			NULL,
			D3D11_SDK_VERSION,
			&scd,
			&pSwapchain,
			&pDevice,
			NULL,
			&m_pImmediateContext);

		// Setup Render Target
		pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&backbufferRenderTexture.pNativeTexture);

		// Create render target with backbuffer address
		pDevice->CreateRenderTargetView(backbufferRenderTexture.pNativeTexture, NULL, &backbufferRenderTexture.m_pRenderTargetView);

		backbufferRenderTexture.m_width = width;
		backbufferRenderTexture.m_height = height;

		// Setup Depth Buffer

		ID3D11Texture2D *pDepthStencil = nullptr;
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		descDepth.SampleDesc.Count = 4;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		HRESULT hr = pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
		if (FAILED(hr))
		{
			MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Create Depth Texture", MB_OK);
		}

		D3D11_DEPTH_STENCIL_DESC dsDesc;
		ZeroMemory(&dsDesc, sizeof(dsDesc));
		// Depth test paraemeters
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		// Stencil test paremeters
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back facing
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create depth stencil state
		ID3D11DepthStencilState *pDSState;
		pDevice->CreateDepthStencilState(&dsDesc, &pDSState);

		// Bind depth stencil state
		m_pImmediateContext->OMSetDepthStencilState(pDSState, 1);
		pDSState->Release();

		// Bind depth-stencil resource using a view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		descDSV.Texture2D.MipSlice = 0;

		// Create the depth stencil view
		hr = pDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthStencilView);
		if (FAILED(hr))
		{
			MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Create Depth Stencil View", MB_OK);
		}

		pDepthStencil->Release();

		m_pImmediateContext->OMSetRenderTargets(1, &backbufferRenderTexture.m_pRenderTargetView, pDepthStencilView);

		// Setup the Viewport

		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = width * 1.0f;
		viewport.Height = height * 1.0f;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		m_pImmediateContext->RSSetViewports(1, &viewport);

		pDevice->CreateDeferredContext(0, &m_pDeferredContext);

		m_pCurrContext = m_pImmediateContext;
	}

	GraphicsDevice::~GraphicsDevice()
	{
		pSwapchain->SetFullscreenState(FALSE, NULL);

		pSwapchain->Release();
		pDepthStencilView->Release();

		m_pImmediateContext->Release();
		m_pDeferredContext->Release();
		pDevice->Release();
	}

	ID3D11DeviceContext* GraphicsDevice::Context() const noexcept
	{
		return m_pCurrContext;
	}

	ID3D11DeviceContext* GraphicsDevice::DeferredContext() const noexcept
	{
		return m_pDeferredContext;
	}

	ID3D11DeviceContext* GraphicsDevice::ImmediateContext() const noexcept
	{
		return m_pImmediateContext;
	}

	void GraphicsDevice::UseImmediateContext() noexcept
	{
		m_pCurrContext = m_pImmediateContext;
	}

	void GraphicsDevice::UseDeferredContext() noexcept
	{ 
		m_pCurrContext = m_pDeferredContext;
	}

	GPUBuffer CreateConstantBuffer(const GraphicsDevice &device, size_t numBytes, void *initialData)
	{
		GPUBuffer gpuBuffer;
		gpuBuffer.numBytes = numBytes;

		D3D11_BUFFER_DESC cbDesc;
		ZeroMemory(&cbDesc, sizeof(cbDesc));

		cbDesc.ByteWidth = numBytes;
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA resourceData;
		if (initialData)
		{
			ZeroMemory(&resourceData, sizeof(resourceData));
			resourceData.pSysMem = initialData;
		}

		HRESULT hr = device.pDevice->CreateBuffer(&cbDesc, initialData ? &resourceData : nullptr, &gpuBuffer.pNativeBuffer);
		if (FAILED(hr))
		{
			MessageBoxA(nullptr, Utils::GetHRErrorString(hr).c_str(), "CreateConstantBuffer", MB_OK);
		}

		return gpuBuffer;
	}

	MappedGPUBuffer MapConstantBuffer(const GraphicsDevice &device, GPUBuffer &gpuBuffer)
	{
		MappedGPUBuffer mappedBuffer;
		mappedBuffer.gpuBuffer = &gpuBuffer;

		D3D11_MAPPED_SUBRESOURCE resource;
		ZeroMemory(&resource, sizeof(resource));

		HRESULT hr = device.Context()->Map(gpuBuffer.pNativeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		if (FAILED(hr))
		{
			MessageBoxA(nullptr, Utils::GetHRErrorString(hr).c_str(), "Map constant buffer", MB_OK);
		}

		mappedBuffer.data = (char *)resource.pData;

		return mappedBuffer;
	}

	void UnmapConstantBuffer(const GraphicsDevice &device, const MappedGPUBuffer &mappedBuffer)
	{
		device.Context()->Unmap(mappedBuffer.gpuBuffer->pNativeBuffer, 0);
	}

	void BindConstantBuffer(const GraphicsDevice &device, const GPUBuffer &gpuBuffer, uint32_t bindSlot)
	{
		device.Context()->VSSetConstantBuffers(bindSlot, 1, &gpuBuffer.pNativeBuffer);
		device.Context()->PSSetConstantBuffers(bindSlot, 1, &gpuBuffer.pNativeBuffer);
	}

	void ClearRenderTarget(const GraphicsDevice &device, const RenderTexture &rtv, float clearColour[])
	{
		device.Context()->ClearRenderTargetView(rtv.m_pRenderTargetView, clearColour);
	}

	void ClearDepth(const GraphicsDevice &device, ID3D11DepthStencilView *depthView)
	{
		device.Context()->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

} // namespace Engine