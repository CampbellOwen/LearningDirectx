#pragma once

#include <DirectXMath.h>
#include "ImGUI\imgui.h"
#include <vector>

#include "GraphicsDevice.h"
#include "Material.h"
#include "Mesh.h"

#define _USE_MATH_DEFINES
#include <math.h>

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
	Entity(std::string name) : m_name(name), m_position(), m_rotation(), m_scale(DirectX::XMFLOAT3(1,1,1)), m_transform(DirectX::XMMatrixIdentity())
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

	virtual void Update(const GraphicsDevice& device)
	{
      m_uiState.x = m_position.x;
      m_uiState.y = m_position.y;
      m_uiState.z = m_position.z;

      m_uiState.rotx = m_rotation.x;
      m_uiState.roty = m_rotation.y;
      m_uiState.rotz = m_rotation.z;

	}

	virtual void DrawUI() 
	{
      ImGui::Begin(m_name.c_str());

      ImGui::Text("Position");
      bool changed = false;
      changed = changed || ImGui::SliderFloat("X", &m_uiState.x, -30.0f, 30.0f);
      changed = changed || ImGui::SliderFloat("Y", &m_uiState.y, -30.0f, 30.0f);
      changed = changed || ImGui::SliderFloat("Z", &m_uiState.z, -30.0f, 30.0f);

      ImGui::Text("Rotation");
      changed = changed || ImGui::SliderFloat("RotationX", &m_uiState.rotx, 0.0f, M_PI * 2.0f);
      changed = changed || ImGui::SliderFloat("RotationY", &m_uiState.roty, 0.0f, M_PI * 2.0f);
      changed = changed || ImGui::SliderFloat("RotationZ", &m_uiState.rotz, 0.0f, M_PI * 2.0f);

      if (changed)
      {
         SetPosition(DirectX::XMFLOAT3(m_uiState.x, m_uiState.y, m_uiState.z));
         SetRotation(DirectX::XMFLOAT3(m_uiState.rotx, m_uiState.roty, m_uiState.rotz));
      }

      ImGui::End();
	}

	DirectX::XMMATRIX GetTransform() const
	{
	    return m_transform;
	}

	virtual void Bind(const GraphicsDevice& device, bool useMaterial);

	std::string m_name;

private:
	void UpdateTransform();

protected:

	struct UIState
	{
      float x, y, z;
      float rotx, roty, rotz;
	};

	UIState m_uiState;

   Material* m_pMaterial { nullptr };
   Mesh* m_pMesh { nullptr };
   DirectX::XMFLOAT3 m_position;
   DirectX::XMFLOAT3 m_rotation;
   DirectX::XMFLOAT3 m_scale;

   DirectX::XMMATRIX m_transform;

	Entity* m_parent { nullptr };
};

} // namespace Engine
