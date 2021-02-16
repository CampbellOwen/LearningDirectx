#pragma once
#include "Material.h"
#include "Entity.h"
#include <d3d11.h>
#include <DirectXMath.h>

namespace Engine
{

struct PerspectiveConstantBuffer
{
	DirectX::XMMATRIX worldTransform;
};

class ThreeTextureMaterial final : public Material
{
public:
	bool Init(const GraphicsDevice& device);

	void Activate(const GraphicsDevice& device, uint32_t shaderResourceStartSlot = 0, uint32_t samplerStartSlot = 0) override;
	void UpdateConstantBuffer(const GraphicsDevice& device, const Entity& entity) override;


private:
	Sampler m_sampler;
};

} // namespace Engine

