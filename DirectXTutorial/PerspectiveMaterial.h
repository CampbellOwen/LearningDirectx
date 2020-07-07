#pragma once
#include "Material.h"
#include <d3d11.h>
#include <DirectXMath.h>

namespace Engine
{

struct PerspectiveConstantBuffer
{
	DirectX::XMMATRIX cameraTransform;
	DirectX::XMMATRIX worldTransform;
};

class PerspectiveMaterial final :
	public Material
{
public:
	bool Init(
		ID3D11Device* device,
		const ShaderInfo& vertexShader,
		const ShaderInfo& pixelShader,
		const D3D11_INPUT_ELEMENT_DESC* inputDesc,
		uint32_t numInputs) override;
	bool UpdateConstantBuffer(ID3D11DeviceContext* deviceContext, const PerspectiveConstantBuffer& buffer);

	void Destroy() override;

	bool AddTexture(ID3D11Device* device, ID3D11Resource* texture);

	void Activate(ID3D11DeviceContext* deviceContext) override;


private:
	ID3D11Buffer* m_pConstantBuffer = nullptr;
	ID3D11ShaderResourceView* m_pTextureView = nullptr;
	ID3D11SamplerState* m_pSamplerState = nullptr;
};

} // namespace Engine

