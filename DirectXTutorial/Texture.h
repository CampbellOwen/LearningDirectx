#pragma once

#include <d3d11.h>

namespace Engine 
{

struct Texture
{
	Texture(ID3D11Device* device, LPCWSTR filename);
	~Texture();

	ID3D11Texture2D* pNativeTexture;
	ID3D11ShaderResourceView* pResourceView;
};

} // namespace Engine
