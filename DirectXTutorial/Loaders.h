#pragma once

#include "Mesh.h"

#include <string>
#include <vector>

namespace Engine
{
	struct PixelData
	{
		uint8_t R, G, B, A;
	};

namespace Loaders
{
	std::vector<Engine::VERTEX> LoadObj(const std::string& filename);
	std::vector<PixelData> LoadImage(LPCWSTR filename);

} // namespace Loaders
} // namespace Engine