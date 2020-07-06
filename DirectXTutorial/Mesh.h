#pragma once
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>

#include <vector>


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
		bool Init(ID3D11Device* device, const ID3D11DeviceContext* deviceContext, const std::vector<VERTEX>& vertices);
		void Destroy(void);
		
		void Render(ID3D11DeviceContext* deviceContext);
		uint32_t NumberVertices(void);

	private:
		ID3D11Buffer* m_pVBuffer = nullptr;
		uint32_t m_numVertices = 0;
	};

}
