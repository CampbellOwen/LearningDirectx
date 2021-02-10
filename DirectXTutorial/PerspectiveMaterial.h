#pragma once
#include "Material.h"
#include <d3d11.h>
#include <DirectXMath.h>

namespace Engine
{

struct PerspectiveConstantBuffer
{
	DirectX::XMMATRIX worldTransform;
};

class PerspectiveMaterial final : public Material
{
public:
	bool Init(const GraphicsDevice& device);

	void Destroy() override;

	void Activate(const GraphicsDevice& device) override;


private:
	ID3D11SamplerState* m_pSamplerState = nullptr;
};

} // namespace Engine

