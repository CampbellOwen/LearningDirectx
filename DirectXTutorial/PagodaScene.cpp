#include "PagodaScene.h"

#include "Loaders.h"
#include "PerspectiveMaterial.h"

namespace Game
{

void PagodaScene::Load(const Engine::GraphicsDevice& device)
{

	const std::vector<Engine::VERTEX> pagodaVerts = Engine::Loaders::LoadObj("F:\\Models\\JapaneseTemple\\model.obj");
	Engine::Mesh* pagodaMesh = new Engine::Mesh();
	pagodaMesh->Init(device.pDevice, device.pImmediateContext, pagodaVerts);

	m_meshes.emplace("Pagoda", pagodaMesh);

	Engine::Texture* pagodaTexture = new Engine::Texture(device.pDevice, L"F:\\Models\\JapaneseTemple\\Textures\\albedo.png");
	m_textures.emplace("Pagoda_Albedo", pagodaTexture);

	Engine::PerspectiveMaterial* material = new Engine::PerspectiveMaterial();
	material->AddTexture(pagodaTexture);
	material->Init(device.pDevice);

	m_materials.emplace("Perspective", material);

	Engine::Entity* pagoda = new Engine::Entity();
	m_entities.emplace("Pagoda", pagoda);

	pagoda->Init(pagodaMesh, material);
}

} // namespace Game