#include "PerspectiveMaterial.h"

#include "Utils.h"

namespace Engine
{

bool PerspectiveMaterial::Init(
	ID3D11Device* device,
	const ShaderInfo& vertexShader,
	const ShaderInfo& pixelShader,
	const D3D11_INPUT_ELEMENT_DESC* inputDesc,
	uint32_t numInputs)
{
	const bool result = Material::Init(device, vertexShader, pixelShader, inputDesc, numInputs);
	if (!result) {
		return false;
	}

	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));

	cbDesc.ByteWidth = sizeof(PerspectiveConstantBuffer);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = device->CreateBuffer(&cbDesc, nullptr, &m_pConstantBuffer);
	if (FAILED(hr)) {
		MessageBoxA(nullptr, Utils::GetHRErrorString(hr).c_str(), "CreateConstantBuffer", MB_OK);
		Destroy();
		return false; 
	}

	return true;
}

void PerspectiveMaterial::Destroy()
{
	Material::Destroy();

	if (m_pConstantBuffer) {
		m_pConstantBuffer->Release();
	}
	if (m_pTextureView) {
		m_pTextureView->Release();
	}
}

bool PerspectiveMaterial::UpdateConstantBuffer(ID3D11DeviceContext* deviceContext, const PerspectiveConstantBuffer& buffer)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	ZeroMemory(&resource, sizeof(resource));

	HRESULT hr = deviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (FAILED(hr)) {
		MessageBoxA(nullptr, Utils::GetHRErrorString(hr).c_str(), "Map constant buffer", MB_OK);
		return false;
	}

	memcpy(resource.pData, &buffer, sizeof(PerspectiveConstantBuffer));

	deviceContext->Unmap(m_pConstantBuffer, 0);

	return true;
}

void PerspectiveMaterial::Activate(ID3D11DeviceContext* deviceContext)
{
	Material::Activate(deviceContext);

	deviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	if (m_pTextureView) {
		deviceContext->PSSetShaderResources(0, 1, &m_pTextureView);
	}
	if (m_pSamplerState) {
		deviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
	}
}

bool PerspectiveMaterial::AddTexture(ID3D11Device* device, ID3D11Resource* texture)
{
	HRESULT hr = device->CreateShaderResourceView(texture, nullptr, &m_pTextureView);
	if (FAILED(hr)) {
		MessageBoxA(nullptr, Utils::GetHRErrorString(hr).c_str(), "Create Shader Resource View", MB_OK);
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

	hr = device->CreateSamplerState(&samplerDesc, &m_pSamplerState);
	if (FAILED(hr)) {
		MessageBoxA(nullptr, Utils::GetHRErrorString(hr).c_str(), "Create Sampler State", MB_OK);
		return false;
	}

	return true;
}

} // namespace Engine
