#pragma once

#include <DirectXMath.h>

#include "Entity.h"

namespace Engine 
{

class Camera : public Entity
{
public:
	Camera(
		const GraphicsDevice& device,
		std::string name,
		float fov = 0.785398f,
		float aspectRatio = 1.3333333f,
		float nearClippingPlane = 1.0f,
		float farClippingPlane = 300.0f);

	void LookAt(DirectX::XMFLOAT3 dir, DirectX::XMFLOAT3 up);

	void Update(const GraphicsDevice& device) override;
	void Bind(const GraphicsDevice& device, bool useMaterial) override;
	void DrawUI() override;

private:

private:

   float m_fov;
   float m_aspectRatio;
   float m_nearClippingPlane;
   float m_farClippingPane;

	GPUBuffer m_gpuBuffer;
};

} // namespace Engine
