#pragma once

#include <d3d11.h>
#include <string>
#include <vector>

#include "Texture.h"

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
		virtual bool Init(
			ID3D11Device* device,
			const ShaderInfo& vertexShader,
			const ShaderInfo& pixelShader,
			const D3D11_INPUT_ELEMENT_DESC* inputDesc,
			uint32_t numInputs );
		virtual void Destroy();

		virtual void Activate(ID3D11DeviceContext* deviceContext);
		void AddSampler(ID3D11SamplerState* samplerState);
		void AddTexture(Texture* texture);

	private:
		ID3D11VertexShader* m_pVertexShader;
		ID3D11PixelShader* m_pPixelShader;
		ID3D11InputLayout* m_pLayout;
		std::vector<Texture*> m_textures;
		std::vector<ID3D11SamplerState*> m_samplerStates;
	};
}