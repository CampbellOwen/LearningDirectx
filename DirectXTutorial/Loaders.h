#pragma once

#include "Mesh.h"

#include <d3d11.h>

#include <string>
#include <vector>

namespace Engine
{
namespace Loaders
{
	std::vector<Engine::VERTEX> LoadObj(const std::string& filename);
	HRESULT LoadImage(
		ID3D11Device* device, 
		LPCWSTR filename, 
		ID3D11Texture2D** texture
	);

} // namespace Loaders
} // namespace Engine