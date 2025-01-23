#pragma once

/**
 * \file D3D11Renderer.h
 * \brief Direct3D 11-based implementation of RendererBase for SnapEngine on Windows.
 */

#include "RendererBase.h"
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h> // For Microsoft::WRL::ComPtr

/**
 * \class D3D11Renderer
 * \brief Concrete renderer using Direct3D 11 on Windows.
 *
 * This class manages the D3D11 device, context, swap chain, and
 * basic rendering pipeline state. It can draw a Model's geometry
 * to the screen. 
 *
 * \note Assumes the caller is on Windows with the appropriate D3D11 libraries.
 */
class D3D11Renderer final : public RendererBase
{
public:
    D3D11Renderer() = default;
    ~D3D11Renderer();

    bool Initialize(void* windowHandle) override;
    void BeginFrame() override;
    void DrawModel(const Model& model) override;
    void EndFrame() override;

    /// \brief Runs basic tests for D3D11Renderer.
    static void test();

private:
    // D3D device, device context, swap chain, etc.
    Microsoft::WRL::ComPtr<ID3D11Device>           m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    m_context;
    Microsoft::WRL::ComPtr<IDXGISwapChain>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

    /// \brief Creates the render target view from the back buffer.
    bool createRenderTarget();
};
