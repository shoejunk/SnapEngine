#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * \class Window
 * \brief Encapsulates a GLFW window with OpenGL context.
 */
class Window
{
public:
    /**
     * \brief Constructs a Window object with a given title and dimensions.
     * \param title Window title.
     * \param width Desired window width (client area).
     * \param height Desired window height (client area).
     */
    Window(const std::string& title, int width, int height);

    /// \brief Destructor. Cleans up GLFW resources.
    ~Window();

    /**
     * \brief Creates the GLFW window and initializes OpenGL context.
     * \return True if creation succeeded, false otherwise.
     */
    bool Create();

    /**
     * \brief Processes window events and swaps buffers.
     * \return False if the window should close, true otherwise.
     */
    bool ProcessMessages();

    /**
     * \brief Gets the underlying GLFW window handle.
     * \return The GLFWwindow pointer.
     */
    GLFWwindow* GetHandle() const { return m_window; }

    /**
     * \brief Gets the current window width.
     * \return The window width in pixels.
     */
    int GetWidth() const { return m_width; }

    /**
     * \brief Gets the current window height.
     * \return The window height in pixels.
     */
    int GetHeight() const { return m_height; }

    /**
     * \brief A static test method to verify window creation logic.
     */
    static void test();

private:
    // Error callback for GLFW
    static void ErrorCallback(int error, const char* description);

    // Window resize callback
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

    std::string m_title;
    int m_width;
    int m_height;
    GLFWwindow* m_window = nullptr;
    bool m_isCreated = false;
};
