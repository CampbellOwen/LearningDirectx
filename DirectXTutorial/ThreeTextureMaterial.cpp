#include "ThreeTextureMaterial.h"

#include "Sampler.h"
#include "Utils.h"

namespace Engine
{

bool ThreeTextureMaterial::Init(const GraphicsDevice& device)
{
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	const Engine::ShaderInfo vertexShader{L"ThreeTexturePhong.hlsl", "VShader"};
	const Engine::ShaderInfo pixelShader{ L"ThreeTexturePhong.hlsl", "PShader" };

	const bool result = Material::Init(device, vertexShader, pixelShader, ied, 4);
	if (!result) {
		return false;
	}
	
	m_sampler.Init(device, AddressMode::WRAP);
	Material::AddSampler(&m_sampler);

	AddGPUBuffer(device, sizeof(PerspectiveConstantBuffer));

	return true;
}

void ThreeTextureMaterial::Activate(const GraphicsDevice &device, uint32_t shaderResourceStartSlot, uint32_t samplerStartSlot)
{
	Material::Activate(device, shaderResourceStartSlot, samplerStartSlot);
}

void ThreeTextureMaterial::UpdateConstantBuffer(const GraphicsDevice& device, const Entity& entity)
{
	Engine::MappedGPUBuffer cbuffer = Engine::MapConstantBuffer(device, m_gpuBuffer);
	{
		Engine::PerspectiveConstantBuffer* pBuffer = reinterpret_cast<Engine::PerspectiveConstantBuffer*>(cbuffer.data);
		pBuffer->worldTransform = entity.GetTransform();
	}
	Engine::UnmapConstantBuffer(device, cbuffer);
	Engine::BindConstantBuffer(device, m_gpuBuffer, 1);
}

} // namespace Engine
