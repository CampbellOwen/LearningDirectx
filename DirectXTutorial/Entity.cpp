#include "Entity.h"

namespace Engine 
{
	 
void Entity::Init(Mesh* mesh, Material* material)
{
	 m_pMesh = mesh;
	 m_pMaterial = material;
}

Material* Entity::GetMaterial()
{
	 return m_pMaterial;
}

void Entity::SetMaterial(Material* material)
{
	 m_pMaterial = material;
}

Mesh* Entity::GetMesh()
{
	 return m_pMesh;
}

void Entity::SetMesh(Mesh* mesh)
{
	 m_pMesh = mesh;
}

void Entity::UpdateTransform()
{
	DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&m_rotation);
	m_transform = DirectX::XMMatrixRotationRollPitchYawFromVector(vec);

	vec = DirectX::XMLoadFloat3(&m_scale);
	m_transform *= DirectX::XMMatrixScalingFromVector(vec);

	vec = DirectX::XMLoadFloat3(&m_position);
	m_transform *= DirectX::XMMatrixTranslationFromVector(vec);

}

///////////////////
// Position Setters
///////////////////

void Entity::SetPosition(Axis axis, float magnitude)
{
	 switch (axis)
	 {
		  case Axis::X:
				m_position.x = magnitude;
				break;
		  case Axis::Y:
				m_position.y = magnitude;
				break;
		  case Axis::Z:
				m_position.z = magnitude;
				break;
	 }
	 UpdateTransform();
}

void Entity::MovePosition(Axis axis, float magnitude)
{
	 switch (axis)
	 {
		  case Axis::X:
				m_position.x = m_position.x + magnitude;
				break;
		  case Axis::Y:
				m_position.y = m_position.y + magnitude;
				break;
		  case Axis::Z:
				m_position.z = m_position.z + magnitude;
				break;
	 }
	 UpdateTransform();
}

void Entity::SetPosition(DirectX::XMFLOAT3 position)
{
	 m_position = position;
	 UpdateTransform();
}

void Entity::MovePosition(DirectX::XMFLOAT3 position)
{
	 DirectX::XMVECTOR vec = DirectX::XMVectorAdd(
		  DirectX::XMLoadFloat3(&m_position), 
		  DirectX::XMLoadFloat3(&position));

	 DirectX::XMStoreFloat3(&m_position, vec);
	 UpdateTransform();
}

///////////////////
// Rotation setters
///////////////////

void Entity::SetRotation(Axis axis, float radians)
{
	 switch (axis)
	 {
		  case Axis::X:
				m_rotation.x = radians;
				break;
		  case Axis::Y:
				m_rotation.y = radians;
				break;
		  case Axis::Z:
				m_rotation.z = radians;
				break;
	 }
	 UpdateTransform();
}

void Entity::ApplyRotation(Axis axis, float radians)
{
	 switch (axis)
	 {
		  case Axis::X:
				m_rotation.x = m_rotation.x + radians;
				break;
		  case Axis::Y:
				m_rotation.y = m_rotation.y + radians;
				break;
		  case Axis::Z:
				m_rotation.z = m_rotation.z + radians;
				break;
	 }
	 UpdateTransform();
}

void Entity::SetRotation(DirectX::XMFLOAT3 rotation)
{
	 m_rotation = rotation;
	 UpdateTransform();
}

void Entity::ApplyRotation(DirectX::XMFLOAT3 rotation)
{
	 DirectX::XMVECTOR vec = DirectX::XMVectorAdd(
		  DirectX::XMLoadFloat3(&m_rotation), 
		  DirectX::XMLoadFloat3(&rotation));

	 DirectX::XMStoreFloat3(&m_rotation, vec);
	 UpdateTransform();
}

////////////////
// Scale setters
////////////////
void Entity::SetScale(Axis axis, float magnitude)
{
	 switch (axis)
	 {
		  case Axis::X:
				m_scale.x = magnitude;
				break;
		  case Axis::Y:
				m_scale.y = magnitude;
				break;
		  case Axis::Z:
				m_scale.z = magnitude;
				break;
	 }
	 UpdateTransform();
}

void Entity::ApplyScale(Axis axis, float magnitude)
{
	 switch (axis)
	 {
		  case Axis::X:
				m_scale.x = m_scale.x + magnitude;
				break;
		  case Axis::Y:
				m_scale.y = m_scale.y + magnitude;
				break;
		  case Axis::Z:
				m_scale.z = m_scale.z + magnitude;
				break;
	 }
	 UpdateTransform();
}

void Entity::SetScale(DirectX::XMFLOAT3 scale)
{
	 m_scale = scale;
	 UpdateTransform();
}


void Entity::ApplyScale(DirectX::XMFLOAT3 scale)
{
	 DirectX::XMVECTOR vec = DirectX::XMVectorAdd(
		  DirectX::XMLoadFloat3(&m_scale), 
		  DirectX::XMLoadFloat3(&scale));

	 DirectX::XMStoreFloat3(&m_scale, vec);
	 UpdateTransform();
}

void Entity::Bind(const GraphicsDevice& device, bool useMaterial)
{
	if (m_pMesh)
	{
		m_pMesh->Activate(device.Context());
	}

	if (m_pMaterial)
	{
      m_pMaterial->UpdateConstantBuffer(device, *this);
		if (useMaterial)
		{
         m_pMaterial->Activate(device);
		}
	}
}

} // namespace Engine