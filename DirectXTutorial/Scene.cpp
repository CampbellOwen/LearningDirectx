#include "Scene.h"

namespace Engine
{

Entity* Scene::AddEntity(std::string id)
{
	m_entities[id] = new Entity();
	return m_entities[id];
}

Entity* Scene::GetEntity(std::string id)
{
	return m_entities[id];
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