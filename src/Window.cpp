#include "Window.h"
#include <iostream>
#include <cassert>

// Error callback for GLFW
void Window::ErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// Window resize callback
void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Constructor
Window::Window(const std::string& title, int width, int height)
    : m_title(title)
    , m_width(width)
    , m_height(height)
    , m_window(nullptr)
    , m_isCreated(false)
{
}

// Destructor
Window::~Window()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

bool Window::Create()
{
    if (m_isCreated)
    {
        return true;
    }

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return false;
    }

    // Make the window's context current
    glfwMakeContextCurrent(m_window);

    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        return false;
    }

    // Set viewport
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);

    // Set callbacks
    glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);

    // Enable vsync
    glfwSwapInterval(1);

    m_isCreated = true;
    return true;
}

bool Window::ProcessMessages()
{
    if (!m_isCreated || !m_window)
    {
        return false;
    }

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Swap buffers and poll events
    glfwSwapBuffers(m_window);
    glfwPollEvents();

    // Return false if the window should close
    return !glfwWindowShouldClose(m_window);
}

void Window::test()
{
    std::cout << "[Window] Running tests...\n";

    // Create a test window
    Window testWindow("Test Window", 800, 600);
    assert(testWindow.Create());

    // Process a few frames
    for (int i = 0; i < 10 && testWindow.ProcessMessages(); ++i)
    {
        // Just testing the window stays open and processes messages
    }

    std::cout << "Window test completed successfully" << std::endl;
}
