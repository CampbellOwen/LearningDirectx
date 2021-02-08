#pragma once

#include <d3d11.h>
#include <string>
#include <vector>

#include "GraphicsDevice.h"
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
			const GraphicsDevice& device,
			const ShaderInfo& vertexShader,
			const ShaderInfo& pixelShader,
			const D3D11_INPUT_ELEMENT_DESC* inputDesc,
			uint32_t numInputs );
		virtual void Destroy();

		virtual void Activate(const GraphicsDevice& device);
		void AddSampler(ID3D11SamplerState* samplerState);
		void AddTexture(Texture* texture);

		void AddGPUBuffer(const GraphicsDevice& device, void* buffer, size_t numBytes);
		void AddGPUBuffer(const GraphicsDevice& device, size_t numBytes);
		GPUBuffer* GetGPUBuffer();


	private:
		ID3D11VertexShader* m_pVertexShader;
		ID3D11PixelShader* m_pPixelShader;
		ID3D11InputLayout* m_pLayout;
		std::vector<Texture*> m_textures;
		std::vector<ID3D11SamplerState*> m_samplerStates;
		GPUBuffer m_gpuBuffer;
	};
}