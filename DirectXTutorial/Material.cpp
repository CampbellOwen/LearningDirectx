#include "Material.h"

#include "Utils.h"

#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace Engine
{

	bool Material::Init(
		const GraphicsDevice &device,
		const ShaderInfo &vertexShader,
		const ShaderInfo &pixelShader,
		const D3D11_INPUT_ELEMENT_DESC *inputDesc,
		uint32_t numInputs)
	{
		HRESULT hr;

		// Vertex Shader
		ID3D10Blob *VS = nullptr;
		ID3DBlob *errorMessages = nullptr;
		hr = D3DCompileFromFile(
			vertexShader.filename,
			nullptr,
			nullptr,
			vertexShader.entryPoint.c_str(),
			"vs_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&VS,
			&errorMessages);
		;
		if (FAILED(hr))
		{
			if (errorMessages)
			{
				auto errorMsg = (const char *)errorMessages->GetBufferPointer();
				MessageBoxA(nullptr, errorMsg, "Vertex Shader Error", MB_OK);
				errorMessages->Release();
			}
			Destroy();
			return false;
		}
		hr = device.pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &m_pVertexShader);
		if (FAILED(hr))
		{
			MessageBoxA(nullptr, "CreateVertexShader error", "Vertex Shader Error", MB_OK);
			Destroy();
			return false;
		}

		// Pixel Shader
		ID3D10Blob *PS = nullptr;
		errorMessages = nullptr;
		hr = D3DCompileFromFile(
			pixelShader.filename,
			0,
			0,
			pixelShader.entryPoint.c_str(),
			"ps_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&PS,
			&errorMessages);
		if (FAILED(hr))
		{
			if (errorMessages)
			{
				auto errorMsg = (const char *)errorMessages->GetBufferPointer();
				MessageBoxA(nullptr, errorMsg, "Pixel Shader Error", MB_OK);
				errorMessages->Release();
			}
			Destroy();
			return false;
		}
		hr = device.pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &m_pPixelShader);
		if (FAILED(hr))
		{
			MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Pixel Shader Error", MB_OK);
			Destroy();
			return false;
		}

		if (inputDesc && numInputs > 0)
		{
			hr = device.pDevice->CreateInputLayout(inputDesc, numInputs, VS->GetBufferPointer(), VS->GetBufferSize(), &m_pLayout);
			if (FAILED(hr))
			{
				MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "CreateInputLayout Error", MB_OK);
				Destroy();
				return false;
			}
		}

		ZeroMemory(&m_gpuBuffer, sizeof(GPUBuffer));

		return true;
	}

	void Material::Activate(const GraphicsDevice &device)
	{
		device.pImmediateContext->VSSetShader(m_pVertexShader, 0, 0);
		device.pImmediateContext->PSSetShader(m_pPixelShader, 0, 0);
		device.pImmediateContext->IASetInputLayout(m_pLayout);

		std::vector<ID3D11ShaderResourceView *> textureViews;
		for (auto &texture : m_textures)
		{
			textureViews.push_back(texture->pResourceView);
		}
		device.pImmediateContext->PSSetShaderResources(0, textureViews.size(), textureViews.data());
		device.pImmediateContext->PSSetSamplers(0, m_samplerStates.size(), m_samplerStates.data());
	}

	void Material::AddSampler(ID3D11SamplerState *samplerState)
	{
		m_samplerStates.push_back(samplerState);
	}

	void Material::AddTexture(Texture *texture)
	{
		m_textures.push_back(texture);
	}

	void Material::Destroy()
	{
		if (m_pVertexShader)
		{
			m_pVertexShader->Release();
		}
		if (m_pPixelShader)
		{
			m_pPixelShader->Release();
		}
		if (m_pLayout)
		{
			m_pLayout->Release();
		}
	}

	void Material::AddGPUBuffer(const GraphicsDevice &device, void *buffer, size_t numBytes)
	{
		m_gpuBuffer = CreateConstantBuffer(device, numBytes, buffer);
	}

	void Material::AddGPUBuffer(const GraphicsDevice &device, size_t numBytes)
	{
		m_gpuBuffer = CreateConstantBuffer(device, numBytes, nullptr);
	}

	GPUBuffer *Material::GetGPUBuffer()
	{
		return &m_gpuBuffer;
	}
} // namespace Engine