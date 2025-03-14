#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

/**
 * \class Camera
 * \brief A camera for 3D scene viewing.
 * 
 * This class provides functionality for a 3D camera including:
 * - Position and orientation control
 * - View and projection matrix generation
 * - Mouse and keyboard input handling
 */
class Camera
{
public:
    enum CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    /**
     * \brief Constructor.
     * \param position Initial camera position.
     */
    Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 3.0f));

    /**
     * \brief Destructor.
     */
    ~Camera();

    /**
     * \brief Process keyboard input.
     * \param direction Movement direction.
     * \param deltaTime Time elapsed since last frame.
     */
    void ProcessKeyboard(CameraMovement direction, float deltaTime);

    /**
     * \brief Process mouse movement.
     * \param xoffset Mouse X movement.
     * \param yoffset Mouse Y movement.
     * \param constrainPitch Whether to constrain pitch.
     */
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    /**
     * \brief Process mouse scroll.
     * \param yoffset Scroll offset.
     */
    void ProcessMouseScroll(float yoffset);

    /**
     * \brief Get the view matrix.
     * \return View matrix.
     */
    glm::mat4 GetViewMatrix() const;

    /**
     * \brief Get camera position.
     * \return Camera position.
     */
    const glm::vec3& GetPosition() const { return m_position; }

    /**
     * \brief Get camera front vector.
     * \return Camera front vector.
     */
    const glm::vec3& GetFront() const { return m_front; }

    /**
     * \brief Get camera up vector.
     * \return Camera up vector.
     */
    const glm::vec3& GetUp() const { return m_up; }

    /**
     * \brief Get camera zoom level.
     * \return Camera zoom level.
     */
    float GetZoom() const { return m_zoom; }

    /**
     * \brief Run unit tests for the Camera class.
     */
    static void test();

    /**
     * \brief Set test mode.
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
     * \brief Update camera vectors.
     */
    void UpdateCameraVectors();

    glm::vec3 m_position;      ///< Camera position
    glm::vec3 m_front;         ///< Camera front vector
    glm::vec3 m_up;           ///< Camera up vector
    glm::vec3 m_right;        ///< Camera right vector
    glm::vec3 m_worldUp;      ///< World up vector

    float m_yaw;              ///< Yaw angle
    float m_pitch;            ///< Pitch angle
    float m_movementSpeed;    ///< Movement speed
    float m_mouseSensitivity; ///< Mouse sensitivity
    float m_zoom;             ///< Zoom level (FOV)

    static bool s_testMode; ///< Test mode flag
};
