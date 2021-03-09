#include "Scene.h"

namespace Engine
{

static struct LightsBuffer
{
	int numLights;
	DirectX::XMFLOAT3 padding;
	LightConstantBuffer lights[Scene::MAX_NUM_LIGHTS];
};

Scene::Scene(const GraphicsDevice& device) : 
	m_lightBuffer(
		CreateConstantBuffer(
			device, 
			sizeof(LightsBuffer),
			nullptr))
{

}

Entity* Scene::AddEntity(std::string id)
{
	m_entities[id] = new Entity(id);
	return m_entities[id];
}

Entity* Scene::GetEntity(std::string id)
{
	return m_entities[id];
}

void Scene::AddCamera(Camera* camera, bool active)
{
	m_cameras.emplace(camera->m_name, camera);

	if (active)
	{
		activeCamera = camera;
	}
}

void Scene::BindCameras(const GraphicsDevice& device)
{
	if (activeCamera)
	{
		activeCamera->Bind(device, false);
	}
}

std::vector<Entity*> Scene::GetEntities()
{
	std::vector<Entity*> entities;
	for (auto& pair : m_entities)
	{
		entities.push_back(pair.second);
	}

	return entities;
}

void Scene::AddLight(Light* light)
{
   assert(m_numLights < MAX_NUM_LIGHTS);
	m_lights[m_numLights++] = light;
}

void Scene::BindLights(const GraphicsDevice& device)
{
	Engine::MappedGPUBuffer cbuffer = Engine::MapConstantBuffer(device, m_lightBuffer);
	{
		LightsBuffer* pBuffer = reinterpret_cast<LightsBuffer*>(cbuffer.data);
		pBuffer->numLights = static_cast<int>(m_numLights);
      for (size_t i = 0; i < m_numLights; i++)
      {
			m_lights[i]->UpdateConstantBuffer(&pBuffer->lights[i]);
      }
	}

	Engine::UnmapConstantBuffer(device, cbuffer);
   BindConstantBuffer(device, m_lightBuffer, 1 /* Light constants always in slot 1 */);
}

Scene::~Scene()
{
	for (auto&& entityPair : m_entities)
	{
		delete entityPair.second;
	}

	for (auto&& materialPair : m_materials)
	{
		materialPair.second->Destroy();
		delete materialPair.second;
	}

	for (auto&& meshPair : m_meshes)
	{
		delete meshPair.second;
	}

	for (auto&& texturePair : m_textures)
	{
		delete texturePair.second;
	}
}

} // namespace Engine