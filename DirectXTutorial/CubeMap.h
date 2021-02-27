#pragma once
#include "Texture.h"

#include <array>

namespace Engine {

class CubeMap : public Texture
{
public:
	void LoadFromFiles(ID3D11Device* device, std::array<LPCWSTR, 6> filename);

};

} // namespace Engine


