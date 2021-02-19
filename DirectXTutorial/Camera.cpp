#include "Camera.h"

namespace Engine
{

static struct ConstantBuffer
{
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMFLOAT4 cameraPos;
	DirectX::XMFLOAT4 lightPos;
};


Camera::Camera(
   const GraphicsDevice& device, 
   std::string name,
   float fov,
   float aspectRatio,
   float nearClippingPlane,
   float farClippingPlane) : Entity(name), 
                             m_fov(fov), 
                             m_aspectRatio(aspectRatio), 
                             m_nearClippingPlane(nearClippingPlane), 
                             m_farClippingPane(farClippingPlane),
                             m_gpuBuffer(CreateConstantBuffer(device, sizeof(ConstantBuffer), nullptr))
{
}

void Camera::LookAt(DirectX::XMFLOAT3 dir, DirectX::XMFLOAT3 up)
{
   assert(false);
}

void Camera::Bind(const GraphicsDevice& device, bool /* useMaterial */)
{
	Engine::MappedGPUBuffer cbuffer = Engine::MapConstantBuffer(device, m_gpuBuffer);
	{
		ConstantBuffer* pBuffer = reinterpret_cast<ConstantBuffer*>(cbuffer.data);
		pBuffer->view = DirectX::XMMatrixInverse(nullptr, GetTransform());
      pBuffer->projection = DirectX::XMMatrixPerspectiveFovLH(
         m_fov,
         m_aspectRatio,
         m_nearClippingPlane,
         m_farClippingPane);
      pBuffer->cameraPos = DirectX::XMFLOAT4(
         m_position.x,
         m_position.y,
         m_position.z,
         1.0f);
      pBuffer->lightPos = DirectX::XMFLOAT4(
         0.0f,
         0.0f,
         0.0f,
         1.0f);
	}
	Engine::UnmapConstantBuffer(device, cbuffer);
   BindConstantBuffer(device, m_gpuBuffer, 0 /* Camera constants always in slot 0 */);
}

void Camera::Update(const GraphicsDevice& device)
{
   Entity::Update(device);
}

void Camera::DrawUI()
{
   Entity::DrawUI();
      ImGui::Begin(m_name.c_str());
      
      ImGui::SliderFloat("FOV", &m_fov, 0.001, (3 * M_PI) / 4);
      
      ImGui::End();
}

}
