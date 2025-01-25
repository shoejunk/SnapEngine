/**
 * \file D3D11Renderer.cpp
 * \brief Implements a basic Direct3D 11 renderer for SnapEngine.
 */
#include "D3D11Renderer.h"
#include "Mesh.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using Microsoft::WRL::ComPtr;

D3D11Renderer::~D3D11Renderer()
{
    // ComPtr automatically releases all resources.
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
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Width = 0;
    swapChainDesc.BufferDesc.Height = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,                 // Default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        nullptr, 0,              // Default feature levels
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &m_swapChain,
        &m_device,
        &featureLevel,
        &m_context
    );

    if (FAILED(hr))
    {
        std::cerr << "[D3D11Renderer] Failed to create device and swap chain. HRESULT: " << std::hex << hr << "\n";
        return false;
    }

    std::cout << "[D3D11Renderer] Device feature level: " << std::hex << featureLevel << "\n";

    if (!createRenderTarget())
    {
        std::cerr << "[D3D11Renderer] Failed to create render target.\n";
        return false;
    }

    // 1. Create Depth-Stencil Buffer and View
    D3D11_TEXTURE2D_DESC depthStencilDesc = {};
    depthStencilDesc.Width = 1280;
    depthStencilDesc.Height = 720;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ComPtr<ID3D11Texture2D> depthStencilBuffer;
    hr = m_device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);
    if (FAILED(hr))
    {
        std::cerr << "[D3D11Renderer] Failed to create depth-stencil buffer. HRESULT: " << std::hex << hr << "\n";
        return false;
    }

    ComPtr<ID3D11DepthStencilView> depthStencilView;
    hr = m_device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, &depthStencilView);
    if (FAILED(hr))
    {
        std::cerr << "[D3D11Renderer] Failed to create depth-stencil view. HRESULT: " << std::hex << hr << "\n";
        return false;
    }

    // Bind depth-stencil view to the output merger stage
    m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), depthStencilView.Get());

    // 2. Create Depth-Stencil State
    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = {};
    depthStencilStateDesc.DepthEnable = TRUE;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

    ComPtr<ID3D11DepthStencilState> depthStencilState;
    hr = m_device->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilState);
    if (FAILED(hr))
    {
        std::cerr << "[D3D11Renderer] Failed to create depth-stencil state. HRESULT: " << std::hex << hr << "\n";
        return false;
    }
    m_context->OMSetDepthStencilState(depthStencilState.Get(), 1);

    // 3. Set Viewport
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<float>(1280);
    viewport.Height = static_cast<float>(720);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_context->RSSetViewports(1, &viewport);

    // Load shaders and create constant buffer
    if (!loadShaders("shaders/BasicVertexShader.cso", "shaders/BasicPixelShader.cso"))
    {
        std::cerr << "[D3D11Renderer] Failed to load shaders.\n";
        return false;
    }

    if (!createConstantBuffer())
    {
        std::cerr << "[D3D11Renderer] Failed to create constant buffer.\n";
        return false;
    }

    std::cout << "[D3D11Renderer] Initialized successfully.\n";
    return true;
#else
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

bool D3D11Renderer::loadShaders(const std::string& vsPath, const std::string& psPath)
{
    // Load Vertex Shader
    std::cout << "[D3D11Renderer] Attempting to load vertex shader from: " << vsPath << "\n";
    std::ifstream vsFile(vsPath, std::ios::binary | std::ios::ate);
    if (!vsFile)
    {
        std::cerr << "[D3D11Renderer] Failed to open " << vsPath << "\n";
        return false;
    }

    std::streamsize vsSize = vsFile.tellg();
    if (vsSize == 0)
    {
        std::cerr << "[D3D11Renderer] Vertex shader file is empty: " << vsPath << "\n";
        return false;
    }
    vsFile.seekg(0, std::ios::beg);

    std::vector<char> vsData((std::istreambuf_iterator<char>(vsFile)), std::istreambuf_iterator<char>());
    std::cout << "[D3D11Renderer] Vertex shader bytecode size: " << vsData.size() << " bytes.\n";
    std::cout << "[D3D11Renderer] Vertex shader bytecode (first 16 bytes): ";
    #undef min
    for (size_t i = 0; i < std::min(vsData.size(), (size_t)16); ++i)
    {
        std::cout << std::hex << (unsigned int)(unsigned char)vsData[i] << " ";
    }
    std::cout << std::dec << "\n";

    HRESULT hr = m_device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &m_vertexShader);
    if (FAILED(hr))
    {
        std::cerr << "[D3D11Renderer] Failed to create vertex shader. HRESULT: " << std::hex << hr << "\n";
        return false;
    }

    // Define Input Layout
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = m_device->CreateInputLayout(layout, ARRAYSIZE(layout), vsData.data(), vsData.size(), &m_inputLayout);
    if (FAILED(hr))
    {
        std::cerr << "[D3D11Renderer] Failed to create input layout. HRESULT: " << std::hex << hr << "\n";
        return false;
    }

    // Load Pixel Shader
    std::cout << "[D3D11Renderer] Attempting to load pixel shader from: " << psPath << "\n";
    std::ifstream psFile(psPath, std::ios::binary | std::ios::ate);
    if (!psFile)
    {
        std::cerr << "[D3D11Renderer] Failed to open " << psPath << "\n";
        return false;
    }

    std::streamsize psSize = psFile.tellg();
    if (psSize == 0)
    {
        std::cerr << "[D3D11Renderer] Pixel shader file is empty: " << psPath << "\n";
        return false;
    }
    psFile.seekg(0, std::ios::beg);

    std::vector<char> psData((std::istreambuf_iterator<char>(psFile)), std::istreambuf_iterator<char>());

    hr = m_device->CreatePixelShader(psData.data(), psData.size(), nullptr, &m_pixelShader);
    if (FAILED(hr))
    {
        std::cerr << "[D3D11Renderer] Failed to create pixel shader. HRESULT: " << std::hex << hr << "\n";
        return false;
    }

    std::cout << "[D3D11Renderer] Shaders loaded successfully.\n";
    return true;
}

bool D3D11Renderer::createConstantBuffer()
{
    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.ByteWidth = sizeof(Transform);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = m_device->CreateBuffer(&cbDesc, nullptr, &m_constantBuffer);
    if (FAILED(hr))
    {
        std::cerr << "[D3D11Renderer] Failed to create constant buffer. HRESULT: " << std::hex << hr << "\n";
        return false;
    }

    std::cout << "[D3D11Renderer] Constant buffer created successfully.\n";
    return true;
}

void D3D11Renderer::BeginFrame()
{
    const float clearColor[4] = { 0.1f, 0.2f, 0.4f, 1.0f };
    m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
    m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
}

void D3D11Renderer::DrawModel(const Model& model)
{
    if (!model.GetMeshCount())
    {
        std::cerr << "[D3D11Renderer] No meshes to draw in the model.\n";
        return;
    }

	// Set the primitive topology
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Transform transform = {};
    XMMATRIX world = XMMatrixIdentity(); // Default world transform
    XMMATRIX view = XMMatrixLookAtLH(
        XMVectorSet(0.0f, 2.0f, -5.0f, 1.0f), // Camera position
        XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),  // Look-at point
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)   // Up vector
    );
    XMMATRIX proj = XMMatrixPerspectiveFovLH(
        XM_PIDIV4,   // Field of view (45 degrees)
        1280.0f / 720.0f, // Aspect ratio
        0.1f,        // Near plane
        100.0f       // Far plane
    );

    transform.worldViewProj = XMMatrixTranspose(world * view * proj);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = m_context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr))
    {
        memcpy(mappedResource.pData, &transform, sizeof(Transform));
        m_context->Unmap(m_constantBuffer.Get(), 0);
    }
    else
    {
        std::cerr << "[D3D11Renderer] Failed to map constant buffer. HRESULT: " << std::hex << hr << "\n";
        return;
    }

    m_context->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
    m_context->IASetInputLayout(m_inputLayout.Get());
    m_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    for (const auto& mesh : model.GetMeshes())
    {
        Mesh gpuMesh;
        if (!gpuMesh.CreateFromModelPart(m_device.Get(), mesh))
        {
            std::cerr << "Failed to create GPU buffers for a mesh.\n";
            continue;
        }

        gpuMesh.Draw(m_context.Get());
    }
}

void D3D11Renderer::EndFrame()
{
    m_swapChain->Present(1, 0);
}

void D3D11Renderer::UpdateConstantBuffer(const DirectX::XMMATRIX& worldViewProj)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = m_context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(hr))
    {
        std::cerr << "[D3D11Renderer] Failed to map constant buffer. HRESULT: " << std::hex << hr << "\n";
        return;
    }

    // Copy the transposed matrix to the constant buffer
    memcpy(mappedResource.pData, &worldViewProj, sizeof(worldViewProj));
    m_context->Unmap(m_constantBuffer.Get(), 0);

    // Bind the constant buffer to the vertex shader
    m_context->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
}

void D3D11Renderer::test()
{
    std::cout << "[D3D11Renderer] Running tests...\n";

    std::unique_ptr<D3D11Renderer> renderer = std::make_unique<D3D11Renderer>();
    bool initResult = renderer->Initialize(nullptr);
    assert(initResult == false && "Expected initialization to fail with null HWND.");

    std::cout << "[D3D11Renderer] Tests passed.\n";
}
