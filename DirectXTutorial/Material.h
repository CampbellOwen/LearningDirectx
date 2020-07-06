#pragma once

#include <d3d11.h>

#include <string>

namespace Engine
{
	struct ShaderInfo
	{
		LPCWSTR filename;
		std::string entryPoint;
	};

	class Material
	{
	public:
		bool Init(
			ID3D11Device* device,
			const ShaderInfo& vertexShader,
			const ShaderInfo& pixelShader,
			const D3D11_INPUT_ELEMENT_DESC* inputDesc,
			uint32_t numInputs );
		virtual void Destroy();

		virtual void Activate(ID3D11DeviceContext* deviceContext);

	private:
		ID3D11VertexShader* m_pVertexShader;
		ID3D11PixelShader* m_pPixelShader;
		ID3D11InputLayout* m_pLayout;
	};
}