#pragma once

#include <array>

#include "CubeMap.h"
#include "Entity.h"
#include "GraphicsDevice.h"
#include "Material.h"
#include "Mesh.h"
#include "Sampler.h"

namespace Engine 
{

class Skybox : public Entity
{
public:

	Skybox(const GraphicsDevice& device, std::string name, std::array<LPCWSTR, 6> cubeMapFiles);
	~Skybox();

private:

	Mesh m_mesh;
	Material m_material;
	CubeMap m_cubemap;
	Sampler m_sampler;
};

}

