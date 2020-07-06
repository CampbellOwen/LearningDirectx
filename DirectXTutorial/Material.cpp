#include "Material.h"

#include "Utils.h"

#include <d3dcompiler.h>

#pragma comment (lib, "d3dcompiler.lib")

namespace Engine
{

	bool Material::Init(
		ID3D11Device* device,
		const ShaderInfo& vertexShader,
		const ShaderInfo& pixelShader,
		const D3D11_INPUT_ELEMENT_DESC* inputDesc,
		uint32_t numInputs)
	{
		HRESULT hr;
		
		// Vertex Shader
		ID3D10Blob* VS = nullptr;
		ID3DBlob* errorMessages = nullptr;
		hr = D3DCompileFromFile(
			vertexShader.filename, 
			nullptr, 
			nullptr, 
			vertexShader.entryPoint.c_str(), 
			"vs_5_0", 
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 
			0, 
			&VS, 
			&errorMessages
		);;
		if (FAILED(hr)) {
			if (errorMessages) {
				auto errorMsg = (const char*)errorMessages->GetBufferPointer();
				MessageBoxA(nullptr, errorMsg, "Vertex Shader Error", MB_OK);
				errorMessages->Release();
			}
			Destroy();
			return false;
		}
		hr = device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &m_pVertexShader);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, "CreateVertexShader error", "Vertex Shader Error", MB_OK);
			Destroy();
			return false;
		}

		// Pixel Shader
		ID3D10Blob* PS = nullptr;
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
			&errorMessages
		);
		if (FAILED(hr)) {
			if (errorMessages) {
				auto errorMsg = (const char*)errorMessages->GetBufferPointer();
				MessageBoxA(nullptr, errorMsg, "Pixel Shader Error", MB_OK);
				errorMessages->Release();
			}
			Destroy();
			return false;
		}
		hr = device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &m_pPixelShader);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Pixel Shader Error", MB_OK);
			Destroy();
			return false;
		}

		hr = device->CreateInputLayout(inputDesc, numInputs, VS->GetBufferPointer(), VS->GetBufferSize(), &m_pLayout);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "CreateInputLayout Error", MB_OK);
			Destroy();
			return false;
		}

		return true;
	}

	void Material::Activate(ID3D11DeviceContext* deviceContext)
	{
		deviceContext->VSSetShader(m_pVertexShader, 0, 0);
		deviceContext->PSSetShader(m_pPixelShader, 0, 0);
		deviceContext->IASetInputLayout(m_pLayout);
	}

	void Material::Destroy()
	{
		if (m_pVertexShader) {
			m_pVertexShader->Release();
		}
		if (m_pPixelShader) {
			m_pPixelShader->Release();
		}
		if (m_pLayout) {
			m_pLayout->Release();
		}
	}
}