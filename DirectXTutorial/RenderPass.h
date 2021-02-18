#pragma once

#include <vector>
#include <d3d11.h>

#include "Entity.h"
#include "GraphicsDevice.h"
#include "Material.h"
#include "RenderTexture.h"
#include "Scene.h"

namespace Engine
{

class RenderPass
{
public:
    RenderPass(Material* material, std::vector<RenderTexture*> outputs);
    RenderPass(const GraphicsDevice& device, Material* material, uint32_t width, uint32_t height, uint32_t numOutputs);
    virtual void Render(const GraphicsDevice& device, Scene* scene);
    void SetClearColour(float colour[4]);
    void EnableClearOutput();
    void DisableClearOutput();
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

protected:
    std::vector<RenderTexture*> m_outputs;
    Material* m_material;
    bool m_ownOutputs { false };
    bool m_clearOutputs { true };
    float m_clearColour[4] { 0.0f, 0.0f, 0.0f, 1.0f };
};

} // namespace Engine
