#pragma once

#include <DirectXMath.h>

namespace Engine 
{

class Camera
{
public:

private:

private:

	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_focusDirection;
	DirectX::XMFLOAT3 m_upDirection;

	DirectX::XMFLOAT4X4 m_viewMatrix;
	DirectX::XMFLOAT4X4 m_projectionMatrix;

	float m_fov;
	float m_aspectRatio;
	float m_nearClippingPlane;
	float m_farClippingPane;


};

} // namespace Engine
