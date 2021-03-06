#pragma once
 
#include <algorithm>
#include <d3d11.h>
#include <stdint.h>
#include <windows.h>

#include "RenderTexture.h"

namespace Engine
{

struct GraphicsDevice
{

	GraphicsDevice(HWND hWnd, uint32_t width, uint32_t height);
	~GraphicsDevice();
	ID3D11DeviceContext* Context() const noexcept;
	ID3D11DeviceContext* ImmediateContext() const noexcept;
	ID3D11DeviceContext* DeferredContext() const noexcept;
	void UseImmediateContext() noexcept;
	void UseDeferredContext() noexcept;

	void UnBindResources() const noexcept;

	ID3D11Device* pDevice = nullptr;
	RenderTexture backbufferRenderTexture;
	IDXGISwapChain* pSwapchain = nullptr;

	ID3D11DepthStencilView* pDepthStencilView = nullptr;
	ID3D11Texture2D* pDepthStencilBuffer = nullptr;
	ID3D11DepthStencilState* pDepthStencilState = nullptr;
	
private:
	ID3D11DeviceContext* m_pCurrContext { nullptr };
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	ID3D11DeviceContext* m_pDeferredContext = nullptr;
};

struct GPUBuffer
{
	ID3D11Buffer* pNativeBuffer;
	size_t numBytes;

	GPUBuffer() : pNativeBuffer(nullptr), numBytes(0)
	{
	}

	GPUBuffer(const GPUBuffer& buffer) : pNativeBuffer(buffer.pNativeBuffer), numBytes(buffer.numBytes)
	{
	}

	GPUBuffer(GPUBuffer&& buffer) noexcept : pNativeBuffer(std::exchange(buffer.pNativeBuffer, nullptr)), numBytes(buffer.numBytes)
	{
	}

	GPUBuffer& operator=(const GPUBuffer& buffer)
	{
		return *this = GPUBuffer(buffer);
	}

	GPUBuffer& operator=(GPUBuffer&& buffer) noexcept
	{
		std::swap(pNativeBuffer, buffer.pNativeBuffer);
		numBytes = buffer.numBytes;
		return *this;
	}

	~GPUBuffer()
	{
		if (pNativeBuffer)
		{
			pNativeBuffer->Release();
		}
	}
};

struct MappedGPUBuffer
{
	GPUBuffer* gpuBuffer;
	char* data;
};

GPUBuffer CreateConstantBuffer(const GraphicsDevice& device, size_t numBytes, void* initialData);
MappedGPUBuffer MapConstantBuffer(const GraphicsDevice& device, GPUBuffer& gpuBuffer);
void UnmapConstantBuffer(const GraphicsDevice& device, const MappedGPUBuffer& mappedBuffer);
void BindConstantBuffer(const GraphicsDevice& device, const GPUBuffer& gpuBuffer, uint32_t bindSlot);

void ClearRenderTarget(const GraphicsDevice& device, const RenderTexture& rtv, float clearColour[]);
void ClearDepth(const GraphicsDevice& device, ID3D11DepthStencilView* depthView);

} // namespace Engine
