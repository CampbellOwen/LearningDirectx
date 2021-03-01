#include "Mesh.h"
#include "Utils.h"

#include <vector>

namespace Engine
{
	static const std::vector<DirectX::XMFLOAT3> s_CubeVertices {
		DirectX::XMFLOAT3{ 0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f, 0.5f },
		{ -0.5f, -0.5f, 0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ 0.5f, 0.5f, -0.5f },
		{ 0.5f, 0.5f, 0.5f },
		{ -0.5f, 0.5f, 0.5f },
		{ -0.5f, 0.5f, -0.5f },
	};

	static const std::vector<uint32_t> s_CubeIndices {
		2,6,5,
		2,5,1,

		1,5,0,
		0,5,4,

		0,4,3,
		3,4,7,

		2,3,7,
		2,7,6,

		4,5,6,
		4,6,7,

		0,2,1,
		0,3,2
	};

	Mesh::~Mesh()
	{
		Destroy();
	}

	bool Mesh::InitCube(const GraphicsDevice& device)
	{
		return Init<DirectX::XMFLOAT3>(device, s_CubeVertices, s_CubeIndices);
	}

	void Mesh::Destroy()
	{
		if (m_pVBuffer) m_pVBuffer->Release();
		if (m_pIndexBuffer) m_pIndexBuffer->Release();
	}

	void Mesh::Activate(ID3D11DeviceContext* deviceContext)
	{
		UINT stride = m_vertexSize;
		UINT offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, &m_pVBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetPrimitiveTopology(m_topology);
	}

	void Mesh::SetTopology(D3D_PRIMITIVE_TOPOLOGY topology)
	{
		m_topology = topology;
	}

	uint32_t Mesh::NumberIndices() {
		return m_numIndices;
	}
}