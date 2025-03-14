#pragma once

#include <string>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Scene.h"

/**
 * \class Window
 * \brief A window for rendering 3D scenes.
 */
class Window
{
public:
    /**
     * \brief Constructor.
     * \param title Window title.
     * \param width Window width.
     * \param height Window height.
     */
    Window(const std::string& title = "SnapEngine", int width = 800, int height = 600);

    /**
     * \brief Create and initialize the window.
     * \return True if window creation was successful, false otherwise.
     */
    bool Create();

    /**
     * \brief Destructor.
     */
    ~Window();

    /**
     * \brief Run the window's main loop.
     */
    void Run();

    /**
     * \brief Close the window.
     */
    void Close();

    /**
     * \brief Process window messages.
     * \return True if window is still open, false if it should close.
     */
    bool ProcessMessages();

    /**
     * \brief Add a model to the scene.
     * \param model The model to add.
     * \param position The position of the model.
     * \param scale The scale of the model.
     * \param rotation The rotation of the model.
     */
    void AddModel(std::shared_ptr<Model> model, const glm::vec3& position = glm::vec3(0.0f), 
                 const glm::vec3& scale = glm::vec3(1.0f), const glm::vec3& rotation = glm::vec3(0.0f));

    /**
     * \brief Get window width.
     * \return Window width.
     */
    int GetWidth() const { return m_width; }

    /**
     * \brief Get window height.
     * \return Window height.
     */
    int GetHeight() const { return m_height; }

    /**
     * \brief Get window title.
     * \return Window title.
     */
    const std::string& GetTitle() const { return m_title; }

    /**
     * \brief Get the scene associated with this window.
     * \return Pointer to the scene.
     */
    Scene* GetScene() const { return m_scene.get(); }

    /**
     * \brief Run unit tests for Window class.
     */
    static void test();

    /**
     * \brief Enable or disable test mode.
     * \param enabled Whether to enable test mode.
     */
    static void SetTestMode(bool enabled) { s_testMode = enabled; }

    /**
     * \brief Check if test mode is enabled.
     * \return Whether test mode is enabled.
     */
    static bool IsTestMode() { return s_testMode; }

private:
    /**
     * \brief Static callback for framebuffer resize events.
     */
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    /**
     * \brief Static callback for keyboard events.
     */
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    /**
     * \brief Static callback for mouse movement events.
     */
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);

    /**
     * \brief Static callback for mouse scroll events.
     */
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    std::string m_title;           ///< Window title
    int m_width;                   ///< Window width
    int m_height;                  ///< Window height
    GLFWwindow* m_window;         ///< GLFW window handle
    std::unique_ptr<Scene> m_scene; ///< Scene to render

    static bool s_testMode;        ///< Test mode flag
};
