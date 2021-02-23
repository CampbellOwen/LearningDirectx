#pragma once
#include "Material.h"
#include <d3d11.h>
#include <DirectXMath.h>

namespace Engine
{

class LightMaterial final : public Material
{
public:
	bool Init(const GraphicsDevice& device);
   void UpdateConstantBuffer(const GraphicsDevice& device, const Entity& entity) override;
};

} // namespace Engine

