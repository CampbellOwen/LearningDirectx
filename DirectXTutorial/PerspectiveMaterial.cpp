#include "PerspectiveMaterial.h"

#include "Utils.h"

namespace Engine
{

bool PerspectiveMaterial::Init(const GraphicsDevice& device)
{
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	const Engine::ShaderInfo vertexShader{L"shaders.hlsl", "VShader"};
	const Engine::ShaderInfo pixelShader{ L"shaders.hlsl", "PShader" };

	const bool result = Material::Init(device, vertexShader, pixelShader, ied, 4);
	if (!result) {
		return false;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = device.pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState);
	if (FAILED(hr)) {
		MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Create Sampler State", MB_OK);
		return false;
	}

	Material::AddSampler(m_pSamplerState);

	AddGPUBuffer(device, sizeof(PerspectiveConstantBuffer));

	return true;
}

void PerspectiveMaterial::Destroy()
{
	Material::Destroy();

	if (m_pSamplerState)
	{
		m_pSamplerState->Release();
		m_pSamplerState = nullptr;
	}
}

void PerspectiveMaterial::Activate(const GraphicsDevice& device)
{
	Material::Activate(device);
}

} // namespace Engine
