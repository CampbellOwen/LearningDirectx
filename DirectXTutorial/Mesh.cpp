#include "Mesh.h"
#include "Utils.h"

#include <vector>

namespace Engine
{
	static const std::vector<DirectX::XMFLOAT3> s_CubeVertices {
		DirectX::XMFLOAT3{ -0.5f, 0.5f, 0.5f }, 
		{ -0.5f,-0.5f, 0.5f },
		{ -0.5f,-0.5f,-0.5f },

		{ -0.5f, 0.5f,-0.5f },
		{ -0.5f,-0.5f,-0.5f },
		{ 0.5f, 0.5f,-0.5f }, 

		{ 0.5f,-0.5f,-0.5f },
		{ -0.5f,-0.5f,-0.5f },
		{ 0.5f,-0.5f, 0.5f },

		{ -0.5f,-0.5f,-0.5f },
		{ 0.5f,-0.5f,-0.5f },
		{ 0.5f, 0.5f,-0.5f },

		{ -0.5f, 0.5f,-0.5f },
		{ -0.5f, 0.5f, 0.5f },
		{ -0.5f,-0.5f,-0.5f },

		{ -0.5f,-0.5f,-0.5f },
		{ -0.5f,-0.5f, 0.5f },
		{ 0.5f,-0.5f, 0.5f },

		{ 0.5f,-0.5f, 0.5f },
		{ -0.5f,-0.5f, 0.5f },
		{ -0.5f, 0.5f, 0.5f },

		{ 0.5f, 0.5f,-0.5f },
		{ 0.5f,-0.5f,-0.5f },
		{ 0.5f, 0.5f, 0.5f },

		{ 0.5f,-0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f },
		{ 0.5f,-0.5f,-0.5f },

		{ -0.5f, 0.5f,-0.5f },
		{ 0.5f, 0.5f,-0.5f },
		{ 0.5f, 0.5f, 0.5f },

		{ -0.5f, 0.5f, 0.5f },
		{ -0.5f, 0.5f,-0.5f },
		{ 0.5f, 0.5f, 0.5f },

		{ 0.5f,-0.5f, 0.5f },
		{ -0.5f, 0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f },
	};
	Mesh::~Mesh()
	{
		Destroy();
	}

	bool Mesh::InitCube(const GraphicsDevice& device)
	{
		return Init<DirectX::XMFLOAT3>(device, s_CubeVertices);
	}

	void Mesh::Destroy()
	{
		if (m_pVBuffer) m_pVBuffer->Release();
	}

	void Mesh::Activate(ID3D11DeviceContext* deviceContext)
	{
		UINT stride = m_vertexSize;
		UINT offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, &m_pVBuffer, &stride, &offset);
		deviceContext->IASetPrimitiveTopology(m_topology);
	}

	void Mesh::SetTopology(D3D_PRIMITIVE_TOPOLOGY topology)
	{
		m_topology = topology;
	}

	uint32_t Mesh::NumberVertices() {
		return m_numVertices;
	}
}