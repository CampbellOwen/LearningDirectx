#include "Sampler.h"
#include "Utils.h"

namespace Engine
{

static D3D11_TEXTURE_ADDRESS_MODE convertAddressMode(AddressMode mode)
{
    switch (mode)
    {
        case AddressMode::BORDER:
            return D3D11_TEXTURE_ADDRESS_BORDER;
        case AddressMode::CLAMP:
            return D3D11_TEXTURE_ADDRESS_CLAMP;
        case AddressMode::MIRROR:
            return D3D11_TEXTURE_ADDRESS_MIRROR;
        case AddressMode::MIRROR_ONCE:
            return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
        case AddressMode::WRAP:
            return D3D11_TEXTURE_ADDRESS_WRAP;
    }
}
    
bool Sampler::Init(const GraphicsDevice& device, AddressMode addressMode)
{
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(samplerDesc));

    D3D11_TEXTURE_ADDRESS_MODE mode = convertAddressMode(addressMode);

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = mode;
    samplerDesc.AddressV = mode;
    samplerDesc.AddressW = mode;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = device.pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState);
	if (FAILED(hr)) {
		MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Create Sampler State", MB_OK);
		return false;
	}
}

} // namespace Engine