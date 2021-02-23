#include "PagodaScene.h"

#include "Camera.h"
#include "ImplicitMesh.h"
#include "Light.h"
#include "LightMaterial.h"
#include "Loaders.h"
#include "PagodaEntity.h"
#include "ThreeTextureMaterial.h"


namespace Game
{

void PagodaScene::Load(const Engine::GraphicsDevice& device)
{

	Engine::Camera* camera = new Engine::Camera(device, "Main Camera");
	camera->MovePosition(Engine::Axis::Z, -20.0f);
	m_entities.emplace("Main Camera", camera);
	AddCamera(camera, true);

	const std::vector<Engine::VERTEX> pagodaVerts = Engine::Loaders::LoadObj("F:\\Models\\JapaneseTemple\\model.obj");
	Engine::Mesh* pagodaMesh = new Engine::Mesh();
	pagodaMesh->Init(device, pagodaVerts);

	m_meshes.emplace("Pagoda", pagodaMesh);

	Engine::Texture* pagodaTexture = new Engine::Texture(); 
	pagodaTexture->InitFromFile(device.pDevice, L"F:\\Models\\JapaneseTemple\\Textures\\albedo.png");
	m_textures.emplace("Pagoda_Albedo", pagodaTexture);

	Engine::Texture* pagodaAO = new Engine::Texture();
	pagodaAO->InitFromFile(device.pDevice, L"F:\\Models\\JapaneseTemple\\Textures\\ao.png");
	m_textures.emplace("Pagoda_AO", pagodaAO);

	Engine::Texture* pagodaMetallic = new Engine::Texture();
	pagodaMetallic->InitFromFile(device.pDevice, L"F:\\Models\\JapaneseTemple\\Textures\\metallic.png");
	m_textures.emplace("Pagoda_Metallic", pagodaMetallic);

	Engine::ThreeTextureMaterial* pagodaMaterial = new Engine::ThreeTextureMaterial();
	pagodaMaterial->AddTexture(pagodaTexture);
	pagodaMaterial->AddTexture(pagodaAO);
	pagodaMaterial->AddTexture(pagodaMetallic);
	pagodaMaterial->Init(device);

	m_materials.emplace("Perspective", pagodaMaterial);

	Engine::Entity* pagoda = new Engine::Entity("Pagoda 1");
	pagoda->SetPosition(DirectX::XMFLOAT3(-6.878f, -8.341f, 8.0f));
	pagoda->SetRotation(Engine::Axis::Y, 2.421f);

	m_entities.emplace("Pagoda", pagoda);

	pagoda->Init(pagodaMesh, pagodaMaterial);

	Engine::Entity* otherPagoda = new Engine::Entity("Pagoda 2");
	m_entities.emplace("Pagoda2", otherPagoda);
	otherPagoda->Init(pagodaMesh, pagodaMaterial);

	otherPagoda->SetPosition(DirectX::XMFLOAT3(20.0f, -8.341f, 50.0f));
	otherPagoda->SetRotation(Engine::Axis::Y, 3.494f);

	Engine::LightMaterial* lightMaterial = new Engine::LightMaterial();
	lightMaterial->Init(device);

	Engine::ImplicitMesh* lightMesh = new Engine::ImplicitMesh(4);
	lightMesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_meshes.emplace("LightMesh", lightMesh);

	Engine::Light* light = new Engine::Light("Light");
	m_entities.emplace("Light", light);
	AddLight(light);
	light->Init(lightMesh, lightMaterial);
	light->SetPosition(Engine::Axis::Z, 10.0f);

	Engine::Light* light2 = new Engine::Light("Light2");
	m_entities.emplace(light2->m_name, light2);
	light2->Init(lightMesh, lightMaterial);
	AddLight(light2);
	light2->SetPosition(Engine::Axis::Z, 0.0f);

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

		Engine::Texture* floorTexture = new Engine::Texture();
		floorTexture->InitFromFile(device.pDevice, L"F:\\Textures\\Final.png");
		m_textures.emplace("Floor", floorTexture);

		Engine::ThreeTextureMaterial* floorMaterial = new Engine::ThreeTextureMaterial();
		floorMaterial->AddTexture(floorTexture);
		floorMaterial->Init(device);

		m_materials.emplace("Floor", floorMaterial);

		Engine::Entity* floorEntity = new Engine::Entity("Floor");
		m_entities.emplace("Floor", floorEntity);
		floorEntity->Init(floorMesh, floorMaterial);
		floorEntity->SetPosition(DirectX::XMFLOAT3(0.0f, -8.341f, 0.0f));
	}

}

} // namespace Game