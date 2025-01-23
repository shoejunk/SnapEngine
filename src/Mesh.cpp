/**
 * \file Mesh.cpp
 * \brief Implements a GPU-side mesh for SnapEngine.
 */
#include "Mesh.h"
#include <cassert>
#include <iostream>

Mesh::~Mesh()
{
    // ComPtr will release resources automatically.
}

bool Mesh::CreateFromModelPart(ID3D11Device* device, const Model::Mesh& srcMeshData)
{
    if (!device)
    {
        std::cerr << "[Mesh] Invalid D3D device.\n";
        return false;
    }

    // In this example, we assume the vertex format is:
    //  - Position (3 floats)
    //  - Normal (3 floats)
    //  - UV (2 floats)
    // so stride = 8 * sizeof(float).
    // If your Model format changes, you must adjust accordingly.
    constexpr UINT FLOATS_PER_VERTEX = 8;
    m_vertexStride = FLOATS_PER_VERTEX * sizeof(float);

    // Create vertex buffer
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.ByteWidth = static_cast<UINT>(srcMeshData.vertices.size() * sizeof(float));
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = srcMeshData.vertices.data();

    HRESULT hr = device->CreateBuffer(&vbDesc, &vbData, m_vertexBuffer.GetAddressOf());
    if (FAILED(hr))
    {
        std::cerr << "[Mesh] Failed to create vertex buffer.\n";
        return false;
    }

    m_vertexCount = static_cast<UINT>(srcMeshData.vertices.size() / FLOATS_PER_VERTEX);

    // Create index buffer
    if (!srcMeshData.indices.empty())
    {
        D3D11_BUFFER_DESC ibDesc = {};
        ibDesc.Usage = D3D11_USAGE_DEFAULT;
        ibDesc.ByteWidth = static_cast<UINT>(srcMeshData.indices.size() * sizeof(unsigned int));
        ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA ibData = {};
        ibData.pSysMem = srcMeshData.indices.data();

        hr = device->CreateBuffer(&ibDesc, &ibData, m_indexBuffer.GetAddressOf());
        if (FAILED(hr))
        {
            std::cerr << "[Mesh] Failed to create index buffer.\n";
            return false;
        }

        m_indexCount = static_cast<UINT>(srcMeshData.indices.size());
    }

    return true;
}

void Mesh::Draw(ID3D11DeviceContext* context) const
{
    if (!context)
    {
        return;
    }
    // Bind vertex buffer
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_vertexStride, &offset);

    // Bind index buffer (if any)
    if (m_indexBuffer && m_indexCount > 0)
    {
        context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        context->DrawIndexed(m_indexCount, 0, 0);
    }
    else
    {
        // No index buffer, draw non-indexed
        context->Draw(m_vertexCount, 0);
    }
}

void Mesh::test()
{
    std::cout << "[Mesh] Running tests...\n";
    // We can't fully test GPU code without a device.
    // We'll just do a minimal check here.
    Mesh mesh;
    bool result = mesh.CreateFromModelPart(nullptr, {});
    // Should fail because device is null:
    assert(result == false && "Expected failure with null device.");

    std::cout << "[Mesh] Tests passed.\n";
}
