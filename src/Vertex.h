#pragma once

#include <bgfx/bgfx.h>
#include <DirectXMath.h>

struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 texCoord;

    static bgfx::VertexLayout GetLayout()
    {
        static bgfx::VertexLayout layout;
        static bool initialized = false;

        if (!initialized)
        {
            layout
                .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
                .end();
            initialized = true;
        }

        return layout;
    }
};
