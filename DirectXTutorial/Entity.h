#pragma once

#include <DirectXMath.h>

#include "Material.h"
#include "Mesh.h"

namespace Engine 
{

enum class Axis
{
	 X,
	 Y,
	 Z
};

class Entity
{

public:
	 void Init(Mesh* mesh, Material* material);

	 Material* GetMaterial();
	 void SetMaterial(Material* material);

	 Mesh* GetMesh();
	 void SetMesh(Mesh* mesh);

	 // Position Setters
	 void SetPosition(Axis axis, float magnitude);
	 void MovePosition(Axis axis, float magnitude);

	 void SetPosition(DirectX::XMFLOAT3 position);
	 void MovePosition(DirectX::XMFLOAT3 position);

	 // Rotation setters
	 void SetRotation(Axis axis, float radians);
	 void ApplyRotation(Axis axis, float radians);

	 void SetRotation(DirectX::XMFLOAT3 rotation);
	 void ApplyRotation(DirectX::XMFLOAT3 rotation);

	 // Scale setters
	 void SetScale(Axis axis, float magnitude);
	 void ApplyScale(Axis axis, float magnitude);

	 void SetScale(DirectX::XMFLOAT3 scale);
	 void ApplyScale(DirectX::XMFLOAT3 scale);

private:
	 Material* m_pMaterial;
	 Mesh* m_pMesh;
	 DirectX::XMFLOAT3 m_position;
	 DirectX::XMFLOAT3 m_rotation;
	 DirectX::XMFLOAT3 m_scale;
};

} // namespace Engine
