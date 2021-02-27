#include "CubeMap.h"

#include "Loaders.h"
#include "Utils.h"

namespace Engine 
{

void CubeMap::LoadFromFiles(ID3D11Device* device, std::array<LPCWSTR, 6> filename)
{
	uint32_t width { 0 };
	uint32_t height { 0 };

	D3D11_SUBRESOURCE_DATA initData[6];
	Loaders::DiskImage loadedImages[6];
	for (int i = 0; i < 6; i++)
	{
		HRESULT hr = Loaders::LoadImageW(filename[i], &loadedImages[i]);
		if (FAILED(hr))
		{
			MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Load Image", MB_OK);
		}

		if (width == 0 || height == 0)
		{
			width = loadedImages[i].width;
			height = loadedImages[i].height;
		}
		else
		{
			assert(width == loadedImages[i].width && height == loadedImages[i].height);
		}

		initData[i].pSysMem = loadedImages[i].spBuffer.get();
		initData[i].SysMemPitch = (loadedImages[i].width * 4);
		initData[i].SysMemSlicePitch = (loadedImages[i].width * loadedImages[i].height);
	}

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));

	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 6;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	HRESULT hr = device->CreateTexture2D(&desc, &initData[0], &pNativeTexture);
	if (FAILED(hr)) {
		MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Create Texure", MB_OK);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = desc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = desc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	hr = device->CreateShaderResourceView(pNativeTexture, &SMViewDesc, &pResourceView);
	if (FAILED(hr))
	{
		MessageBoxA(nullptr, Utils::GetHRErrorString(hr).c_str(), "Create Texture Resource View", MB_OK);
	}
}

} // namespace Engine

