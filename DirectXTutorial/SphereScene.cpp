#include "SphereScene.h"

#include "ConstantPbrMaterial.h"
#include "Loaders.h"

namespace Game {

void SphereScene::Load(const Engine::GraphicsDevice& device)
{

	Engine::Camera* camera = new Engine::Camera(device, "Main Camera");
	camera->MovePosition(Engine::Axis::Z, -20.0f);
	m_entities.emplace(camera->m_name, camera);

	AddCamera(camera, true);

	auto sphereMeshData = Engine::Loaders::LoadObj("F:\\Models\\sphere_highpoly.obj");
	Engine::Mesh* sphereMesh = new Engine::Mesh();
	sphereMesh->Init(device, sphereMeshData.first, sphereMeshData.second);

	m_meshes.emplace("Sphere", sphereMesh);

	Engine::ConstantPbrMaterial* material = new Engine::ConstantPbrMaterial();
	material->Init(device);

	m_materials.emplace("ConstantPbr", material);

	Engine::Entity* sphereEntity = new Engine::Entity("Sphere");
	sphereEntity->SetScale(DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f));
	sphereEntity->SetPosition(Engine::Axis::Z, -15.0f);
	
	m_entities.emplace(sphereEntity->m_name, sphereEntity);
	sphereEntity->Init(sphereMesh, material);


	Engine::Light* light = new Engine::Light("Light1");
	m_entities.emplace(light->m_name, light);
	AddLight(light);
	light->SetPosition(DirectX::XMFLOAT3(5.0f, 5.0f, -30.0f));
}

} // namespace Game
