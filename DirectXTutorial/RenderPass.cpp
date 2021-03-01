#include "RenderPass.h"

#include "Camera.h"
#include "Utils.h"

namespace Engine
{

static constexpr uint32_t MAX_OUTPUTS = 8;


RenderPass::RenderPass(Material* material, std::vector<RenderTexture*> outputs)
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
        if (m_clearOutputs)
        {
           ClearRenderTarget(device, *output, m_clearColour);
        }
    }

    device.Context()->OMSetRenderTargets(m_outputs.size(), renderTargets.data(), device.pDepthStencilView);

    for (auto& entity : entities)
    {
        entity->Bind(device, useEntityMaterial);

        Engine::Mesh* mesh = entity->GetMesh();
        if (mesh)
        {
           device.Context()->DrawIndexed(mesh->NumberIndices(), 0, 0);
        }
    }

    device.UnBindResources();
}

void RenderPass::SetClearColour(float colour[4])
{
   for (int i = 0; i < 4; i++)
   {
      m_clearColour[i] = colour[i];
   }
}

void RenderPass::EnableClearOutput()
{
   m_clearOutputs = true;
}

void RenderPass::DisableClearOutput()
{
   m_clearOutputs = false;
}

} // namespace Engine
