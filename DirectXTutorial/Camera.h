#pragma once

#include <DirectXMath.h>
#include <memory>

#include "Entity.h"
#include "Skybox.h"

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

	void AddSkybox(std::shared_ptr<Skybox> skybox);
	Skybox* GetSkybox();

private:

private:

   float m_fov;
   float m_aspectRatio;
   float m_nearClippingPlane;
   float m_farClippingPane;

	std::shared_ptr<Skybox> m_skybox;

	GPUBuffer m_gpuBuffer;
};

} // namespace Engine
