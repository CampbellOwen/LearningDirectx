#include "PagodaEntity.h"
#include "ImGUI\imgui.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Engine 
{

void PagodaEntity::Update()
{
	m_uiState.x = m_position.x;
	m_uiState.y = m_position.y;
	m_uiState.z = m_position.z;

	m_uiState.rotx = m_rotation.x;
	m_uiState.roty = m_rotation.y;
	m_uiState.rotz = m_rotation.z;
}

void PagodaEntity::DrawUI()
{
   ImGui::Begin("Pagoda controls");

	ImGui::Text("Position");
	bool changed = false;
	changed = changed || ImGui::SliderFloat("X", &m_uiState.x, -30.0f, 30.0f);
	changed = changed || ImGui::SliderFloat("Y", &m_uiState.y, -30.0f, 30.0f);
	changed = changed || ImGui::SliderFloat("Z", &m_uiState.z, -30.0f, 30.0f);

	ImGui::Text("Rotation");
	changed = changed || ImGui::SliderFloat("Rotation X", &m_uiState.rotx, 0.0f, M_PI * 2.0f);
	changed = changed || ImGui::SliderFloat("Rotation Y", &m_uiState.roty, 0.0f, M_PI * 2.0f);
	changed = changed || ImGui::SliderFloat("Rotation Z", &m_uiState.rotz, 0.0f, M_PI * 2.0f);

	if (changed)
	{
		SetPosition(DirectX::XMFLOAT3(m_uiState.x, m_uiState.y, m_uiState.z));
		SetRotation(DirectX::XMFLOAT3(m_uiState.rotx, m_uiState.roty, m_uiState.rotz));
	}

   ImGui::End();
}

} // namespace Engine
