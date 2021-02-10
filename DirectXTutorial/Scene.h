#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Entity.h"
#include "GraphicsDevice.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"

namespace Engine
{

class Scene
{
public:

	virtual void Load(const GraphicsDevice& device) = 0;

	Entity* AddEntity(std::string id);
	Entity* GetEntity(std::string id);

	std::vector<Entity*> GetEntities();

	~Scene();

protected:
	std::unordered_map<std::string, Entity*> m_entities;
	std::unordered_map<std::string, Material*> m_materials;
	std::unordered_map<std::string, Mesh*> m_meshes;
	std::unordered_map<std::string, Texture*> m_textures;
};


} // namespace Engine
