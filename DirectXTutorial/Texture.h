#pragma once

#include <d3d11.h>

namespace Engine 
{

struct Texture
{
	void InitFromFile(ID3D11Device* device, LPCWSTR filename);
	~Texture();

	ID3D11Texture2D* pNativeTexture { nullptr };
	ID3D11ShaderResourceView* pResourceView { nullptr };
};

} // namespace Engine
