#pragma once

#include "Mesh.h"

#include <d3d11.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace Engine
{
namespace Loaders
{

struct DiskImage
{
	std::unique_ptr<BYTE[]> spBuffer;
	uint32_t width, height;
};

	std::pair<std::vector<Engine::VERTEX>, std::vector<uint32_t>> LoadObj(const std::string& filename);
	HRESULT LoadImage( LPCWSTR filename, DiskImage* imageOut );

} // namespace Loaders
} // namespace Engine