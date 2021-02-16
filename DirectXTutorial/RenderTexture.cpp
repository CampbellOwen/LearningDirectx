#include "RenderTexture.h"
#include "Utils.h"

namespace Engine
{

RenderTexture::RenderTexture(const GraphicsDevice& device, uint32_t width, uint32_t height)
{
    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    HRESULT hr = device.pDevice->CreateTexture2D(&textureDesc, nullptr, &pNativeTexture);
    if (FAILED(hr)) {
        MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Creating render texture", MB_OK);
    }

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    hr = device.pDevice->CreateRenderTargetView(pNativeTexture, &renderTargetViewDesc, &m_pRenderTargetView);
    if (FAILED(hr)) {
        MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Creating render texture view", MB_OK);
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    hr = device.pDevice->CreateShaderResourceView(pNativeTexture, &shaderResourceViewDesc, &pResourceView);
    if (FAILED(hr)) {
        MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Creating shader resource view", MB_OK);
    }
}

RenderTexture::~RenderTexture()
{
    if (m_pRenderTargetView)
    {
        m_pRenderTargetView->Release();
        m_pRenderTargetView = nullptr;
    }

    Texture::~Texture();
}

ID3D11RenderTargetView* RenderTexture::RenderTargetView() const noexcept
{
    return m_pRenderTargetView;
}

} // namespace Engine
