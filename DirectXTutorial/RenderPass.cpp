#include "RenderPass.h"

#include "Utils.h"

namespace Engine
{

static constexpr uint32_t MAX_OUTPUTS = 8;


RenderPass::RenderPass(const GraphicsDevice& device, Material* material, std::vector<RenderTexture*> outputs)
{
    m_material = material;
    m_outputs = outputs;
}

RenderPass::RenderPass(const GraphicsDevice& device, Material* material, uint32_t width, uint32_t height, uint32_t numOutputs)
{
    if (numOutputs > MAX_OUTPUTS) { 
        MessageBoxA(nullptr, "Too many output textures, max is 8", "Texture Error", MB_OK);
    }

    m_material = material;

    m_outputs.reserve(numOutputs); 

    for (int i = 0; i < numOutputs; i++)
    {
        m_outputs.push_back(new RenderTexture(device, width, height));
    }

    // Since we created the RenderTextures, we have to free them
    m_ownOutputs = true;

}

void RenderPass::Render(const GraphicsDevice& device, std::vector<Entity*> entities)
{
    bool useEntityMaterial = m_material == nullptr;
    if (!useEntityMaterial)
    {
        m_material->Activate(device);
    }

    std::vector<ID3D11RenderTargetView*> renderTargets;
    renderTargets.reserve(m_outputs.size());
    for (auto& output : m_outputs)
    {
        renderTargets.push_back(output->m_pRenderTargetView);
    }

    device.Context()->OMSetRenderTargets(m_outputs.size(), renderTargets.data(), device.pDepthStencilView);

	for (auto& entity : entities)
	{
		Engine::Mesh* mesh = entity->GetMesh();
		if (!mesh) {
			continue;
		}

        if (useEntityMaterial)
        {
            entity->Bind(device);
        }
        else
        {
            mesh->Activate(device.Context());
            m_material->UpdateConstantBuffer(device, *entity);
        }
		device.Context()->Draw(mesh->NumberVertices(), 0);
	}

    device.UnBindResources();
}

} // namespace Engine
