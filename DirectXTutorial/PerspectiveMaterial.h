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
	bool SetConstantBuffer(ID3D11Device* device, const PerspectiveConstantBuffer& buffer);
	bool UpdateConstantBuffer(ID3D11DeviceContext* deviceContext, const PerspectiveConstantBuffer& buffer);

	void Destroy() override
	{
		Material::Destroy();

		if (m_pConstantBuffer) {
			m_pConstantBuffer->Release();
		}
	}

	void Activate(ID3D11DeviceContext* deviceContext) override;


private:
	ID3D11Buffer* m_pConstantBuffer = nullptr;
};

} // namespace Engine

