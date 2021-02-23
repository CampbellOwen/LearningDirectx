#include "LightMaterial.h"

#include "Entity.h"
#include "Utils.h"

namespace Engine
{

struct LightConstantBuffer
{
	DirectX::XMMATRIX worldTransform;
};

bool LightMaterial::Init(const GraphicsDevice& device)
{
	const Engine::ShaderInfo vertexShader{ L"light.hlsl", "VS" };
	const Engine::ShaderInfo pixelShader{ L"light.hlsl", "PS" };

	const bool result = Material::Init(device, vertexShader, pixelShader, nullptr, 0);
	if (!result) {
		return false;
	}

	AddGPUBuffer(device, sizeof(LightConstantBuffer));

	return true;
}

void LightMaterial::UpdateConstantBuffer(const GraphicsDevice& device, const Entity& entity)
{
	Engine::MappedGPUBuffer cbuffer = Engine::MapConstantBuffer(device, m_gpuBuffer);
	{
		LightConstantBuffer* pBuffer = reinterpret_cast<Engine::LightConstantBuffer*>(cbuffer.data);
		pBuffer->worldTransform = entity.GetTransform();
	}
	Engine::UnmapConstantBuffer(device, cbuffer);
	Engine::BindConstantBuffer(device, m_gpuBuffer, 2 /* Model constants in slot 2*/);
}

} // namespace Engine
