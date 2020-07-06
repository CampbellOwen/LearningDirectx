#include "Mesh.h"
#include "Utils.h"

namespace Engine
{
	Mesh::~Mesh()
	{
		Destroy();
	}

	bool Mesh::Init(ID3D11Device* device, const ID3D11DeviceContext* deviceContext, const std::vector<VERTEX>& vertices)
	{
		HRESULT hr;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = (sizeof(VERTEX) * vertices.size());
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		const D3D11_SUBRESOURCE_DATA vertex_data{vertices.data(), 0, 0};

		hr = device->CreateBuffer(&bd, &vertex_data, &m_pVBuffer);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Mesh Error", MB_OK);
			if (m_pVBuffer) {
				m_pVBuffer->Release();
			}

			return false;
		}

		m_numVertices = vertices.size();

		return true;
	}

	void Mesh::Destroy()
	{
		if (m_pVBuffer) m_pVBuffer->Release();
	}

	void Mesh::Render(ID3D11DeviceContext* deviceContext)
	{
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, &m_pVBuffer, &stride, &offset);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	uint32_t Mesh::NumberVertices() {
		return m_numVertices;
	}
}