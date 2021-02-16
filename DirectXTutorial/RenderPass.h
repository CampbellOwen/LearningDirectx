#pragma once

#include <vector>
#include <d3d11.h>

#include "Entity.h"
#include "GraphicsDevice.h"
#include "Material.h"
#include "RenderTexture.h"

namespace Engine
{

class RenderPass
{
public:
    RenderPass(const GraphicsDevice& device, Material* material, std::vector<RenderTexture*> outputs);
    RenderPass(const GraphicsDevice& device, Material* material, uint32_t width, uint32_t height, uint32_t numOutputs);
    void Render(const GraphicsDevice& device, std::vector<Entity*> entities);
    ~RenderPass()
    {
        if (m_ownOutputs)
        {
            for (auto& texture : m_outputs)
            {
                delete texture;
            }
        }
    }

private:

    std::vector<RenderTexture*> m_outputs;
    Material* m_material;
    bool m_ownOutputs { false };
};

} // namespace Engine
