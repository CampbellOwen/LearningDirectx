#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

#include "Camera.h"
#include "Entity.h"
#include "GraphicsDevice.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"

namespace Engine
{

class Scene
{
public:

	static constexpr size_t MAX_NUM_LIGHTS = 8;

	Scene(const GraphicsDevice& device);

	virtual void Load(const GraphicsDevice& device) = 0;

	Entity* AddEntity(std::string id);
	Entity* GetEntity(std::string id);
	void AddCamera(Camera* camera, bool active = true);
	void ActivateCamera(Camera* camera)
	{
		activeCamera = camera;
	}
	void BindCameras(const GraphicsDevice& device);

	void AddLight(Light* light);
	void BindLights(const GraphicsDevice& device);

	std::vector<Entity*> GetEntities();

	~Scene();

protected:
	std::unordered_map<std::string, Entity*> m_entities;
	std::unordered_map<std::string, Material*> m_materials;
	std::unordered_map<std::string, Mesh*> m_meshes;
	std::unordered_map<std::string, Texture*> m_textures;
	std::unordered_map<std::string, Camera*> m_cameras;

	std::array<Light*, MAX_NUM_LIGHTS> m_lights;
	size_t m_numLights { 0 };



	GPUBuffer m_lightBuffer;
	Camera* activeCamera { nullptr };
};


} // namespace Engine
