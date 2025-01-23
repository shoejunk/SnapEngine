#pragma once

/**
 * \file Mesh.h
 * \brief Encapsulates GPU buffers (vertex, index) for a 3D mesh in SnapEngine.
 */

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

#include "Model.h" // For Model::Mesh data

/**
 * \class Mesh
 * \brief Holds Direct3D 11 buffers and metadata for a single 3D mesh.
 *
 * This class is responsible for creating GPU buffers from CPU mesh data
 * (vertex and index arrays) and issuing draw calls for that mesh.
 */
class Mesh
{
public:
    Mesh() = default;
    ~Mesh();

    /**
     * \brief Creates Direct3D buffers from a Model::Mesh struct.
     * \param device A D3D11 device pointer.
     * \param srcMeshData The CPU-side mesh data from a Model.
     * \return True if buffers were created successfully.
     */
    bool CreateFromModelPart(ID3D11Device* device, const Model::Mesh& srcMeshData);

    /**
     * \brief Issues a draw call for this mesh using the provided device context.
     * \param context A D3D11 device context for rendering.
     */
    void Draw(ID3D11DeviceContext* context) const;

    /**
     * \brief Basic test for the Mesh class.
     */
    static void test();

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
    UINT m_vertexStride = 0;
    UINT m_vertexCount  = 0;
    UINT m_indexCount   = 0;
};
