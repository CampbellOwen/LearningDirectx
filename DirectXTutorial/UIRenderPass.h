#pragma once
#include "RenderPass.h"

namespace Engine 
{

class UIRenderPass final : public RenderPass
{
public:
   UIRenderPass(std::vector<RenderTexture*> outputs);
   void Render(const GraphicsDevice& device, std::vector<Entity*> entities) override;
};

}

