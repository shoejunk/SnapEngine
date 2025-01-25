#pragma once

#include "RendererBase.h"
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h> // For Microsoft::WRL::ComPtr
#include <DirectXMath.h> // For matrix math

using namespace DirectX;

class D3D11Renderer final : public RendererBase
{
public:
    D3D11Renderer() = default;
    ~D3D11Renderer();

    bool Initialize(void* windowHandle) override;
    void BeginFrame() override;
    void DrawModel(const Model& model) override;
    void EndFrame() override;
    void UpdateConstantBuffer(const DirectX::XMMATRIX& worldViewProj);

    static void test();

private:
    bool createRenderTarget();
    bool loadShaders(const std::string& vsPath, const std::string& psPath);
    bool createConstantBuffer(); // New function

    struct Transform
    {
        XMMATRIX worldViewProj;
    };

    Microsoft::WRL::ComPtr<ID3D11Device>           m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    m_context;
    Microsoft::WRL::ComPtr<IDXGISwapChain>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11VertexShader>     m_vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>      m_pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>      m_inputLayout;

    // New member for constant buffer
    Microsoft::WRL::ComPtr<ID3D11Buffer>           m_constantBuffer;
};
