#pragma once

#include "GraphicsDevice.h"
#include "Texture.h"

#include <cstdint>
#include <d3d11.h>

namespace Engine
{

class RenderTexture : public Texture
{
public:
    RenderTexture(const GraphicsDevice& device, uint32_t width, uint32_t height);
    ~RenderTexture();

    ID3D11RenderTargetView* RenderTargetView() const noexcept;
private:
    ID3D11RenderTargetView* m_pRenderTargetView;
};

} // namespace Engine

