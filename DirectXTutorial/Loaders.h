#pragma once

#include "Mesh.h"

#include <d3d11.h>

#include <memory>
#include <string>
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

	std::vector<Engine::VERTEX> LoadObj(const std::string& filename);
	HRESULT LoadImage( LPCWSTR filename, DiskImage* imageOut );

} // namespace Loaders
} // namespace Engine