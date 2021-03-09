#include "ConstantPbrMaterial.h"

#include "Entity.h"

namespace Engine {

static struct ConstantBuffer
{
	DirectX::XMMATRIX worldTransform;
	DirectX::XMFLOAT3 albedo;
	float roughness;
	int useLambert;
	int useDisney;
	int useEarlHammon;
};

bool ConstantPbrMaterial::Init(const GraphicsDevice& device)
{
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	const Engine::ShaderInfo vertexShader{L"ConstantPbr.hlsl", "VShader"};
	const Engine::ShaderInfo pixelShader{ L"ConstantPbr.hlsl", "PShader" };

	const bool result = Material::Init(device, vertexShader, pixelShader, ied, 4);
	if (!result) {
		return false;
	}

	AddGPUBuffer(device, sizeof(ConstantBuffer));

	return true;
}

void ConstantPbrMaterial::UpdateConstantBuffer(const GraphicsDevice& device, const Entity& entity)
{
	Engine::MappedGPUBuffer cbuffer = Engine::MapConstantBuffer(device, m_gpuBuffer);
	{
		ConstantBuffer* pBuffer = reinterpret_cast<ConstantBuffer*>(cbuffer.data);
		pBuffer->albedo = m_albedo;
		pBuffer->roughness = m_roughness;
		pBuffer->worldTransform = entity.GetTransform();
		pBuffer->useLambert = m_radioChoice == 0;
		pBuffer->useDisney = m_radioChoice == 1;
		pBuffer->useEarlHammon = m_radioChoice == 2;
	}
	Engine::UnmapConstantBuffer(device, cbuffer);
	Engine::BindConstantBuffer(device, m_gpuBuffer, 2);
}

void ConstantPbrMaterial::DrawUI()
{
      ImGui::Begin("ConstantPbrMaterial Controls");

      ImGui::SliderFloat("Roughness", &m_roughness, 0.0f, 1.0f);

      ImGui::ColorEdit3("Albedo", &m_albedo.x);

		ImGui::Text("Diffuse BRDF");
		ImGui::RadioButton("Lambertian", &m_radioChoice, 0);
		ImGui::RadioButton("Disney", &m_radioChoice, 1);
		ImGui::RadioButton("Earl Hammon", &m_radioChoice, 2);

      ImGui::End();
}

}
