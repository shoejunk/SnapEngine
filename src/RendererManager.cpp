/**
 * \file RendererManager.cpp
 * \brief Implements the RendererManager class.
 */
#include "RendererManager.h"
#include "D3D11Renderer.h"
#include <iostream>
#include <cassert>

void RendererManager::createObjects()
{
    // Example logic: if the JSON objects have something like
    //   {"class": "renderer", "type": "d3d11"},
    // we create a D3D11Renderer. Here, we just do a default.

    std::cout << "[RendererManager] Creating a D3D11Renderer by default.\n";
    m_renderer = std::make_unique<D3D11Renderer>();
    // Typically, you'd also call m_renderer->Initialize(windowHandle)
    // but you probably want to do that after Window creation.
}

void RendererManager::test()
{
    std::cout << "[RendererManager] Running tests...\n";
    {
        RendererManager rm;
        rm.createObjects();
        RendererBase* r = rm.GetRenderer();
        assert(r != nullptr && "Renderer should be created.");

        // Optional: dynamic_cast to D3D11Renderer, call test on it, etc.
        if (auto d3dRenderer = dynamic_cast<D3D11Renderer*>(r))
        {
            d3dRenderer->test();
        }
    }
    std::cout << "[RendererManager] Tests passed.\n";
}
