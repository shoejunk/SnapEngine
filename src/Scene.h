#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Model.h"
#include "Camera.h"
#include "Shader.h"

/**
 * \struct SceneObject
 * \brief A structure to hold a model and its transform data.
 */
struct SceneObject
{
    std::shared_ptr<Model> model;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
};

/**
 * \class Scene
 * \brief A 3D scene containing models and a camera.
 */
class Scene
{
public:
    /**
     * \brief Constructor.
     */
    Scene();

    /**
     * \brief Destructor.
     */
    ~Scene();

    /**
     * \brief Update the scene.
     * \param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime);

    /**
     * \brief Render the scene.
     */
    void Render();

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
     * \brief Handle keyboard input.
     * \param key The key that was pressed.
     * \param scancode The scancode of the key.
     * \param action The action (press, release, repeat).
     * \param mods Modifier keys that were held.
     */
    void OnKeyInput(int key, int scancode, int action, int mods);

    /**
     * \brief Handle mouse movement.
     * \param xpos The new x position of the mouse.
     * \param ypos The new y position of the mouse.
     */
    void OnMouseMove(double xpos, double ypos);

    /**
     * \brief Handle mouse scroll.
     * \param xoffset The x scroll offset.
     * \param yoffset The y scroll offset.
     */
    void OnMouseScroll(double xoffset, double yoffset);

    /**
     * \brief Get all models in the scene.
     * \return Vector of scene objects.
     */
    const std::vector<SceneObject>& GetModels() const { return m_objects; }

    /**
     * \brief Get the scene's camera.
     * \return Pointer to the camera.
     */
    Camera* GetCamera() const { return m_camera.get(); }

    /**
     * \brief Run unit tests for Scene class.
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
    std::unique_ptr<Camera> m_camera;           ///< Scene camera
    std::unique_ptr<Shader> m_shader;           ///< Scene shader
    std::vector<SceneObject> m_objects;         ///< Scene objects
    bool m_firstMouse;                          ///< First mouse movement flag
    double m_lastX;                             ///< Last mouse X position
    double m_lastY;                             ///< Last mouse Y position

    static bool s_testMode;                     ///< Test mode flag
};
