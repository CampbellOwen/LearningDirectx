#pragma once

#include <d3d11.h>
#include <string>
#include <vector>

#include "GraphicsDevice.h"
#include "Sampler.h"
#include "Texture.h"

namespace Engine
{
	class Entity;
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

		virtual void Activate(const GraphicsDevice& device, uint32_t shaderResourceStartSlot = 0, uint32_t samplerStartSlot = 0);
		void AddSampler(Sampler* sampler);
		void AddTexture(Texture* texture);

		void AddGPUBuffer(const GraphicsDevice& device, void* buffer, size_t numBytes);
		void AddGPUBuffer(const GraphicsDevice& device, size_t numBytes);
		GPUBuffer* GetGPUBuffer();

		virtual void UpdateConstantBuffer(const GraphicsDevice& device, const Entity& entity) 
		{ 

		}


	protected:
		GPUBuffer m_gpuBuffer;
	private:
		ID3D11VertexShader* m_pVertexShader;
		ID3D11PixelShader* m_pPixelShader;
		ID3D11InputLayout* m_pLayout;
		std::vector<Texture*> m_textures;
		std::vector<Sampler*> m_samplers;
	};
}