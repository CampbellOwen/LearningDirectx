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
		ID3D11DeviceContext* deviceContext, 
		LPCWSTR filename, 
		ID3D11Resource** texture
	);

} // namespace Loaders
} // namespace Engine