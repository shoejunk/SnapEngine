#pragma once

/**
 * \file RendererManager.h
 * \brief A manager for creating and owning a RendererBase instance in SnapEngine.
 */

#include "ManagerBase.h"
#include "RendererBase.h"
#include <memory>

/**
 * \class RendererManager
 * \brief A manager that creates and controls a RendererBase instance.
 *
 * Example usage:
 *  - Parse JSON or user input to decide on a specific renderer (e.g., "D3D11")
 *  - Create that renderer, call Initialize(windowHandle)
 *  - Provide access to the renderer instance to the rest of the engine
 */
class RendererManager : public ManagerBase
{
public:
    RendererManager() = default;

    /**
     * \brief Creates the renderer from stored JSON data, or a default if none.
     * 
     * Expects the JSON to have something like {"class": "renderer", "type": "d3d11"}.
     */
    void createObjects() override;

    /**
     * \brief Gets the current renderer instance.
     */
    RendererBase* GetRenderer() const { return m_renderer.get(); }

    /**
     * \brief Basic test routine for RendererManager.
     */
    static void test();

private:
    std::unique_ptr<RendererBase> m_renderer;
};
