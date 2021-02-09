#include "PagodaScene.h"

#include "Loaders.h"
#include "PerspectiveMaterial.h"
#include "LightMaterial.h"
#include "ImplicitMesh.h"

namespace Game
{

void PagodaScene::Load(const Engine::GraphicsDevice& device)
{

	const std::vector<Engine::VERTEX> pagodaVerts = Engine::Loaders::LoadObj("F:\\Models\\JapaneseTemple\\model.obj");
	Engine::Mesh* pagodaMesh = new Engine::Mesh();
	pagodaMesh->Init(device, pagodaVerts);

	m_meshes.emplace("Pagoda", pagodaMesh);

	Engine::Texture* pagodaTexture = new Engine::Texture(device.pDevice, L"F:\\Models\\JapaneseTemple\\Textures\\albedo.png");
	m_textures.emplace("Pagoda_Albedo", pagodaTexture);

	Engine::PerspectiveMaterial* pagodaMaterial = new Engine::PerspectiveMaterial();
	pagodaMaterial->AddTexture(pagodaTexture);
	pagodaMaterial->Init(device);

	m_materials.emplace("Perspective", pagodaMaterial);

	Engine::Entity* pagoda = new Engine::Entity();
	m_entities.emplace("Pagoda", pagoda);

	pagoda->Init(pagodaMesh, pagodaMaterial);

	Engine::Entity* otherPagoda = new Engine::Entity();
	m_entities.emplace("Pagoda2", otherPagoda);
	otherPagoda->Init(pagodaMesh, pagodaMaterial);

	otherPagoda->SetPosition(DirectX::XMFLOAT3(20.0f, -8.341f, 50.0f));
	otherPagoda->SetRotation(Engine::Axis::Y, 3.494f);

	Engine::LightMaterial* lightMaterial = new Engine::LightMaterial();
	lightMaterial->Init(device);

	Engine::ImplicitMesh* lightMesh = new Engine::ImplicitMesh(4);
	lightMesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_meshes.emplace("LightMesh", lightMesh);

	Engine::Entity* light = new Engine::Entity();
	m_entities.emplace("Light", light);
	light->Init(lightMesh, lightMaterial);

	{ // Add floor
		const std::vector<Engine::VERTEX> floorVerts{
			{
				DirectX::XMFLOAT3(-200.0f, 0.0f, -200.0f), // Position;
				DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), // Normal;
				DirectX::XMFLOAT2(0.0f, 50.0f), // TexCoord;
				DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), // Color;
			},
			{
				DirectX::XMFLOAT3(-200.0f, 0.0f, 200.0f), // Position;
				DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), // Normal;
				DirectX::XMFLOAT2(0.0f, 0.0f), // TexCoord;
				DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), // Color;
			},
			{
				DirectX::XMFLOAT3(200.0f, 0.0f, -200.0f), // Position;
				DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), // Normal;
				DirectX::XMFLOAT2(50.0f, 50.0f), // TexCoord;
				DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), // Color;
			},
			{
				DirectX::XMFLOAT3(200.0f, 0.0f, 200.0f), // Position;
				DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), // Normal;
				DirectX::XMFLOAT2(50.0f, 0.0f), // TexCoord;
				DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), // Color;
			},
		};

		Engine::Mesh* floorMesh = new Engine::Mesh();
		floorMesh->Init(device, floorVerts);
		floorMesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		m_meshes.emplace("Floor", floorMesh);

		Engine::Texture* floorTexture = new Engine::Texture(device.pDevice, L"F:\\Textures\\Final.png");
		m_textures.emplace("Floor", floorTexture);

		Engine::PerspectiveMaterial* floorMaterial = new Engine::PerspectiveMaterial();
		floorMaterial->AddTexture(floorTexture);
		floorMaterial->Init(device);

		m_materials.emplace("Floor", floorMaterial);

		Engine::Entity* floorEntity = new Engine::Entity();
		m_entities.emplace("Floor", floorEntity);
		floorEntity->Init(floorMesh, floorMaterial);
		floorEntity->SetPosition(DirectX::XMFLOAT3(0.0f, -8.341f, 0.0f));
	}

}

} // namespace Game