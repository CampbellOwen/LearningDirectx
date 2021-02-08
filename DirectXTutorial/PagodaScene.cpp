#include "PagodaScene.h"

#include "Loaders.h"
#include "PerspectiveMaterial.h"
#include "LightMaterial.h"

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

	Engine::PerspectiveMaterial* material = new Engine::PerspectiveMaterial();
	material->AddTexture(pagodaTexture);
	material->Init(device);

	m_materials.emplace("Perspective", material);

	Engine::Entity* pagoda = new Engine::Entity();
	m_entities.emplace("Pagoda", pagoda);

	pagoda->Init(pagodaMesh, material);

	Engine::Entity* otherPagoda = new Engine::Entity();
	m_entities.emplace("Pagoda2", otherPagoda);
	otherPagoda->Init(pagodaMesh, material);

	otherPagoda->SetPosition(DirectX::XMFLOAT3(20.0f, -8.341f, 50.0f));
	otherPagoda->SetRotation(Engine::Axis::Y, 3.494f);

	Engine::LightMaterial* lightMaterial = new Engine::LightMaterial();
	lightMaterial->Init(device);

	Engine::Entity* light = new Engine::Entity();
	m_entities.emplace("Light", light);
	light->Init(nullptr, lightMaterial);

}

} // namespace Game