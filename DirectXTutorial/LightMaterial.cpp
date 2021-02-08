#include "LightMaterial.h"

#include "Utils.h"

namespace Engine
{

bool LightMaterial::Init(const GraphicsDevice& device)
{
	const Engine::ShaderInfo vertexShader{ L"light.hlsl", "VS" };
	const Engine::ShaderInfo pixelShader{ L"light.hlsl", "PS" };

	const bool result = Material::Init(device, vertexShader, pixelShader, nullptr, 0);
	if (!result) {
		return false;
	}

	return true;
}

void LightMaterial::Activate(const GraphicsDevice& device)
{
	device.pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	Material::Activate(device);
}

}
