#pragma once

#include "Texture.h"

#include <cstdint>
#include <d3d11.h>

namespace Engine
{

struct GraphicsDevice;

struct RenderTexture : public Texture
{
public:
    RenderTexture(){ }
    RenderTexture(ID3D11RenderTargetView* pRTV, ID3D11Texture2D* pTexture2D, uint32_t width, uint32_t height);
    RenderTexture(const GraphicsDevice& device, uint32_t width, uint32_t height);
    ~RenderTexture();

    ID3D11RenderTargetView* m_pRenderTargetView { nullptr };
    uint32_t m_width { 0 };
    uint32_t m_height { 0 };
};

} // namespace Engine

