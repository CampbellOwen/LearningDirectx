#include "PerspectiveMaterial.h"

#include "Utils.h"

bool Engine::PerspectiveMaterial::SetConstantBuffer(ID3D11Device* device, const PerspectiveConstantBuffer& buffer)
{
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));

	cbDesc.ByteWidth = sizeof(buffer);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = &buffer;

	HRESULT hr = device->CreateBuffer(&cbDesc, &initData, &m_pConstantBuffer);
	if (FAILED(hr)) {
		MessageBoxA(nullptr, Engine::Utils::GetLastErrorAsString(hr).c_str(), "CreateConstantBuffer", MB_OK);
		Destroy();
		return false;
	}

	return true;
}

bool Engine::PerspectiveMaterial::UpdateConstantBuffer(ID3D11DeviceContext* deviceContext, const PerspectiveConstantBuffer& buffer)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	ZeroMemory(&resource, sizeof(resource));

	HRESULT hr = deviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (FAILED(hr)) {
		MessageBoxA(nullptr, Engine::Utils::GetLastErrorAsString(hr).c_str(), "Map constant buffer", MB_OK);
		return false;
	}

	memcpy(resource.pData, &buffer, sizeof(PerspectiveConstantBuffer));

	deviceContext->Unmap(m_pConstantBuffer, 0);

	return true;
}

void Engine::PerspectiveMaterial::Activate(ID3D11DeviceContext* deviceContext)
{
	Engine::Material::Activate(deviceContext);

	deviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
}
