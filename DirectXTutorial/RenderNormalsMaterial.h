#pragma once
#include "Material.h"

namespace Engine
{

class RenderNormalsMaterial final : public Material
{
public:
    bool Init(const GraphicsDevice& device);

    void Activate(const GraphicsDevice& device, uint32_t shaderResourceStartSlot = 0, uint32_t samplerStartSlot = 0) override;
    void UpdateConstantBuffer(const GraphicsDevice& device, const Entity& entity) override;
};

} // namespace Engine
