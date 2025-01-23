/**
 * \file D3D11Renderer.cpp
 * \brief Implements a basic Direct3D 11 renderer for SnapEngine.
 */
#include "D3D11Renderer.h"
#include "Mesh.h"
#include <cassert>
#include <iostream>

using Microsoft::WRL::ComPtr;

D3D11Renderer::~D3D11Renderer()
{
    // ComPtr will auto-release D3D objects. 
}

bool D3D11Renderer::Initialize(void* windowHandle)
{
#ifdef _WIN32
    HWND hwnd = static_cast<HWND>(windowHandle);
    if (!hwnd)
    {
        std::cerr << "[D3D11Renderer] Invalid window handle.\n";
        return false;
    }

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = 0;   // Use window's client area
    swapChainDesc.BufferDesc.Height = 0;  
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,                 // default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        nullptr, 0,              // default feature levels
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &m_swapChain,
        &m_device,
        &featureLevel,
        &m_context
    );

    if (FAILED(hr))
    {
        std::cerr << "[D3D11Renderer] Failed to create device and swap chain.\n";
        return false;
    }

    if (!createRenderTarget())
    {
        std::cerr << "[D3D11Renderer] Failed to create render target.\n";
        return false;
    }

    std::cout << "[D3D11Renderer] Initialized successfully.\n";
    return true;
#else
    // Non-Windows path
    std::cerr << "[D3D11Renderer] Only supported on Windows.\n";
    return false;
#endif
}

bool D3D11Renderer::createRenderTarget()
{
    ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                        reinterpret_cast<void**>(backBuffer.GetAddressOf()));
    if (FAILED(hr))
    {
        return false;
    }

    hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTargetView);
    return SUCCEEDED(hr);
}

void D3D11Renderer::BeginFrame()
{
    // Clear the back buffer to a blue-ish color
    const float clearColor[4] = { 0.1f, 0.2f, 0.4f, 1.0f };
    m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
    m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
}

void D3D11Renderer::DrawModel(const Model& model)
{
    size_t meshCount = model.GetMeshCount();
    std::cout << "[D3D11Renderer] Drawing Model with " 
              << meshCount << " mesh(es).\n";

    // Placeholder logic (in a real scenario you'd upload to GPU or fetch cached Mesh objects).
    // for (size_t i = 0; i < meshCount; ++i)
    // {
    //     // If you add a public GetMesh(i) to Model, you can retrieve Model::Mesh data here.
    //     // Then you'd create or bind GPU buffers, set up shaders, and call Draw() in D3D.
    // }
}

void D3D11Renderer::EndFrame()
{
    // Present the swap chain with vsync off (0) or on (1)
    m_swapChain->Present(1, 0);
}

void D3D11Renderer::test()
{
    std::cout << "[D3D11Renderer] Running tests...\n";

    // Minimal test: just instantiate with a null HWND to see if it fails gracefully.
    std::unique_ptr<D3D11Renderer> renderer = std::make_unique<D3D11Renderer>();
    bool initResult = renderer->Initialize(nullptr);
    // We expect false, since the window handle is invalid on Windows.
    assert(initResult == false && "Expected initialization to fail with null HWND.");

    std::cout << "[D3D11Renderer] Tests passed.\n";
}
