#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>

#include "GraphicsDevice.h"
#include "Utils.h"

namespace Engine
{

	struct VERTEX
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 TexCoord;
		DirectX::XMFLOAT4 Color;
	};

	class Mesh
	{
	public:
		~Mesh(void);

		//bool Init(const ID3D11Device* device, const ID3D11DeviceContext* deviceContext, const std::string& filename);
		template <class T>
		bool Init(const GraphicsDevice& device, const std::vector<T>& vertices, const std::vector<uint32_t>& indices)
		{
			HRESULT hr;
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = (sizeof(T) * vertices.size());
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			const D3D11_SUBRESOURCE_DATA vertex_data { vertices.data(), 0, 0 };

			hr = device.pDevice->CreateBuffer(&bd, &vertex_data, &m_pVBuffer);
			if (FAILED(hr))
			{
				MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Mesh Error", MB_OK);
				if (m_pVBuffer)
				{
					m_pVBuffer->Release();
				}

				return false;
			}

			D3D11_BUFFER_DESC indexBufferDesc;
			ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

			indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			indexBufferDesc.ByteWidth = sizeof(uint32_t) * indices.size();
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			D3D11_SUBRESOURCE_DATA indexData;
			indexData.pSysMem = indices.data();
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

			hr = device.pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
			if (FAILED(hr))
			{
				MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Mesh Error", MB_OK);
				if (m_pVBuffer)
				{
					m_pVBuffer->Release();
				}
				if (m_pIndexBuffer)
				{
					m_pIndexBuffer->Release();
				}

				return false;
			}

			m_numIndices = indices.size();
			m_vertexSize = sizeof(T);

			return true;
		}


		bool InitCube(const GraphicsDevice& device);
		void Destroy(void);
		
		void Activate(ID3D11DeviceContext* deviceContext);
		void SetTopology(D3D_PRIMITIVE_TOPOLOGY topology);
		virtual size_t NumberIndices(void);


	private:
		ID3D11Buffer* m_pVBuffer { nullptr };
		ID3D11Buffer* m_pIndexBuffer { nullptr };
		D3D_PRIMITIVE_TOPOLOGY m_topology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };
		size_t m_vertexSize { 0 };
	protected:
		size_t m_numIndices = 0;
	};

}
