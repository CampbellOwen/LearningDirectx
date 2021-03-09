#pragma once
#include "Material.h"

#include <DirectXMath.h>

namespace Engine {

class ConstantPbrMaterial : public Material
{
public:
	bool Init(const GraphicsDevice& device);

	void UpdateConstantBuffer(const GraphicsDevice& device, const Entity& entity) override;

	void DrawUI() override;

private:
	DirectX::XMFLOAT3 m_albedo { DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) };
	float m_roughness { 0.5 };
	int m_radioChoice { 0 };
};

} // namespace Material

