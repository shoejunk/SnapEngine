#pragma once

/**
 * \file RendererBase.h
 * \brief Defines the abstract base class for rendering in SnapEngine.
 */

#include "Model.h"

/**
 * \class RendererBase
 * \brief Abstract base class for a rendering system.
 *
 * Defines the basic interface that a renderer should implement
 * (initialization, frame management, and drawing of Models).
 */
class RendererBase
{
public:
    /// \brief Virtual destructor for proper cleanup.
    virtual ~RendererBase() = default;

    /**
     * \brief Initializes the renderer with a given native window handle.
     * \param windowHandle A platform-specific window pointer (HWND on Windows).
     * \return True if initialized successfully, false otherwise.
     */
    virtual bool Initialize(void* windowHandle) = 0;

    /**
     * \brief Begins rendering a frame (e.g., clears buffers).
     */
    virtual void BeginFrame() = 0;

    /**
     * \brief Draws a Model (or one of its meshes).
     * \param model A reference to the Model to be drawn.
     */
    virtual void DrawModel(const Model& model) = 0;

    /**
     * \brief Ends rendering a frame (e.g., presents swap chain).
     */
    virtual void EndFrame() = 0;

    /**
     * \brief Runs basic tests for the renderer.
     *
     * In a real scenario, this might need a real window to test properly.
     */
    static void test();
};
