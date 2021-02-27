#include "Skybox.h"

namespace Engine
{

Skybox::Skybox(const GraphicsDevice& device, std::string name, std::array<LPCWSTR, 6> cubeMapFiles) : Entity(name)
{
	m_mesh.InitCube(device);
	m_cubemap.LoadFromFiles(device.pDevice, cubeMapFiles);

	D3D11_INPUT_ELEMENT_DESC ied[] = { {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} };
	const Engine::ShaderInfo vertexShader{L"Skybox.hlsl", "VShader"};
	const Engine::ShaderInfo pixelShader{ L"Skybox.hlsl", "PShader" };
	m_material.Init(device, vertexShader, pixelShader, ied, 1);

	m_sampler.Init(device, AddressMode::WRAP);
	m_material.AddSampler(&m_sampler);
	m_material.AddTexture(&m_cubemap);

	Entity::Init(&m_mesh, &m_material);
}

Skybox::~Skybox()
{
	m_mesh.Destroy();
	m_material.Destroy();
}


} // namespace Engine
