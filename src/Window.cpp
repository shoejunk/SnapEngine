#include "Window.h"
#include <iostream>
#include <stdexcept>

// Initialize static members
bool Window::s_testMode = false;

Window::Window(const std::string& title, int width, int height)
    : m_title(title)
    , m_width(width)
    , m_height(height)
    , m_window(nullptr)
{
    // Skip window creation in test mode
    if (s_testMode)
    {
        std::cout << "Window creation skipped in test mode\n";
        return;
    }

    // Initialize GLFW
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    if (!Create())
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    // Create scene
    m_scene = std::make_unique<Scene>();

    // Set callbacks
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetCursorPosCallback(m_window, mouseCallback);
    glfwSetScrollCallback(m_window, scrollCallback);

    // Configure OpenGL
    glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
    if (m_window != nullptr)
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }
}

bool Window::Create()
{
    // Create GLFW window
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return false;
    }

    // Make the window's context current
    glfwMakeContextCurrent(m_window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    // Set viewport
    glViewport(0, 0, m_width, m_height);

    // Set up window resize callback
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    });

    return true;
}

void Window::Run()
{
    if (s_testMode || m_window == nullptr)
        return;

    // Main render loop
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(m_window))
    {
        // Calculate delta time
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Update scene
        m_scene->Update(deltaTime);

        // Render scene
        m_scene->Render();

        // Swap buffers and poll events
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void Window::Close()
{
    if (m_window != nullptr)
    {
        glfwSetWindowShouldClose(m_window, true);
    }
}

void Window::AddModel(std::shared_ptr<Model> model, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation)
{
    if (m_scene)
    {
        m_scene->AddModel(model, position, scale, rotation);
    }
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win != nullptr)
    {
        win->m_width = width;
        win->m_height = height;
        glViewport(0, 0, width, height);
    }
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win != nullptr && win->m_scene)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
            return;
        }
        win->m_scene->OnKeyInput(key, scancode, action, mods);
    }
}

void Window::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win != nullptr && win->m_scene)
    {
        win->m_scene->OnMouseMove(xpos, ypos);
    }
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win != nullptr && win->m_scene)
    {
        win->m_scene->OnMouseScroll(xoffset, yoffset);
    }
}

bool Window::ProcessMessages()
{
    if (s_testMode || m_window == nullptr)
    {
        return true;
    }

    // Process events
    glfwPollEvents();

    // Check if window should close
    return !glfwWindowShouldClose(m_window);
}

void Window::test()
{
    std::cout << "\nRunning Window tests...\n";

    // Enable test mode for this test
    SetTestMode(true);

    // Test window creation
    Window window("Test Window", 800, 600);
    assert(window.m_title == "Test Window" && "Window title not set correctly");
    assert(window.m_width == 800 && "Window width not set correctly");
    assert(window.m_height == 600 && "Window height not set correctly");

    // Test model addition
    auto model = std::make_shared<Model>();
    window.AddModel(model);
    assert(!window.GetScene()->GetModels().empty() && "Failed to add model to window");

    // Disable test mode
    SetTestMode(false);

    std::cout << "Window tests passed!\n";
}
