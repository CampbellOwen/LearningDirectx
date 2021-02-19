#include "RenderNormalsMaterial.h"

#include <DirectXMath.h>

#include "Entity.h"

namespace Engine
{

static struct ConstantBuffer
{
    DirectX::XMMATRIX worldTransform;
};

bool RenderNormalsMaterial::Init(const GraphicsDevice& device)
{
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	const Engine::ShaderInfo vertexShader{L"RenderNormals.hlsl", "VShader"};
	const Engine::ShaderInfo pixelShader{ L"RenderNormals.hlsl", "PShader" };

	const bool result = Material::Init(device, vertexShader, pixelShader, ied, 4);
	if (!result) {
		return false;
	}
	
	AddGPUBuffer(device, sizeof(ConstantBuffer));

	return true;
}

void RenderNormalsMaterial::Activate(const GraphicsDevice &device, uint32_t shaderResourceStartSlot, uint32_t samplerStartSlot)
{
	Material::Activate(device, shaderResourceStartSlot, samplerStartSlot);
	Engine::BindConstantBuffer(device, m_gpuBuffer, 2);
}

void RenderNormalsMaterial::UpdateConstantBuffer(const GraphicsDevice& device, const Entity& entity)
{
	Engine::MappedGPUBuffer cbuffer = Engine::MapConstantBuffer(device, m_gpuBuffer);
	{
		ConstantBuffer* pBuffer = reinterpret_cast<Engine::ConstantBuffer*>(cbuffer.data);
		pBuffer->worldTransform = entity.GetTransform();
	}
	Engine::UnmapConstantBuffer(device, cbuffer);
}

} // namespace Engine
