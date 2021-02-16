#pragma once

#include "GraphicsDevice.h"

#include <d3d11.h>

namespace Engine
{

enum class AddressMode
{
    WRAP,
    MIRROR,
    CLAMP,
    BORDER,
    MIRROR_ONCE
};

class Sampler
{
public:
    ~Sampler()
    {
        if (m_pSamplerState) m_pSamplerState->Release();
    }

    bool Init(const GraphicsDevice& device, AddressMode addressMode);
    ID3D11SamplerState* RawSampler() const noexcept
    {
        return m_pSamplerState;
    }

private:
    ID3D11SamplerState* m_pSamplerState { nullptr };
};

} // namespace Engine
