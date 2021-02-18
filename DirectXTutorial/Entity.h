#pragma once

#include <DirectXMath.h>
#include <vector>

#include "GraphicsDevice.h"
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
	Entity() : m_position(), m_rotation(), m_scale(DirectX::XMFLOAT3(1,1,1)), m_transform(DirectX::XMMatrixIdentity())
	{}
	
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

	void SetParent(Entity* parent)
	{
	    m_parent = parent;
	}

	Entity* GetParent()
	{
	    return m_parent;
	}

	virtual void Update()
	{
	}

	virtual void DrawUI() 
	{
	}

	DirectX::XMMATRIX GetTransform() const
	{
	    return m_transform;
	}

	void Bind(const GraphicsDevice& device);

private:
	void UpdateTransform();

protected:
   Material* m_pMaterial { nullptr };
   Mesh* m_pMesh { nullptr };
   DirectX::XMFLOAT3 m_position;
   DirectX::XMFLOAT3 m_rotation;
   DirectX::XMFLOAT3 m_scale;

   DirectX::XMMATRIX m_transform;

	Entity* m_parent { nullptr };
};

} // namespace Engine
