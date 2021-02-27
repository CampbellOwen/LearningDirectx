#include "Texture.h"

#include "Loaders.h"
#include "Utils.h"

namespace Engine 
{

void Texture::InitFromFile(ID3D11Device* device, LPCWSTR filename)
{
	Loaders::DiskImage loadedImage;
	HRESULT hr = Loaders::LoadImageW(filename, &loadedImage);

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));

	desc.Width = loadedImage.width;
	desc.Height = loadedImage.height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = loadedImage.spBuffer.get();
	initData.SysMemPitch = (loadedImage.width * 4);
	initData.SysMemSlicePitch = (loadedImage.width * loadedImage.height);

	hr = device->CreateTexture2D(&desc, &initData, &pNativeTexture);
	if (FAILED(hr)) {
		MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Create Texure", MB_OK);
	}

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