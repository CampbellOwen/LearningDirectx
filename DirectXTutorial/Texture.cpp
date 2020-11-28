#include "Texture.h"

#include "Loaders.h"
#include "Utils.h"

namespace Engine 
{

Texture::Texture(ID3D11Device* device, LPCWSTR filename)
{
	pNativeTexture = nullptr;
	HRESULT hr = Loaders::LoadImageW(device, filename, &pNativeTexture);

	if (FAILED(hr)) {
		MessageBoxA(nullptr, Utils::GetHRErrorString(hr).c_str(), "Create Texture", MB_OK);
	}

	pResourceView = nullptr;
	hr = device->CreateShaderResourceView(pNativeTexture, nullptr, &pResourceView);
	if (FAILED(hr)) {
		MessageBoxA(nullptr, Utils::GetHRErrorString(hr).c_str(), "Create Texture Resource View", MB_OK);
	}
}

Texture::~Texture()
{
	if (pNativeTexture)
	{
		pNativeTexture->Release();
		pNativeTexture = nullptr;
	}

	if (pResourceView)
	{
		pResourceView->Release();
		pResourceView = nullptr;
	}
}

} // namespace Engine