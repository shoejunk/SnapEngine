#include "Camera.h"
#include <iostream>
#include <cassert>

// Initialize static members
bool Camera::s_testMode = false;

Camera::Camera(const glm::vec3& position)
    : m_position(position)
    , m_front(glm::vec3(0.0f, 0.0f, -1.0f))
    , m_up(glm::vec3(0.0f, 1.0f, 0.0f))
    , m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f))
    , m_yaw(YAW)
    , m_pitch(PITCH)
    , m_movementSpeed(SPEED)
    , m_mouseSensitivity(SENSITIVITY)
    , m_zoom(ZOOM)
{
    UpdateCameraVectors();
}

Camera::~Camera()
{
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;
    if (direction == FORWARD)
        m_position += m_front * velocity;
    if (direction == BACKWARD)
        m_position -= m_front * velocity;
    if (direction == LEFT)
        m_position -= m_right * velocity;
    if (direction == RIGHT)
        m_position += m_right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw   += xoffset;
    m_pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    if (m_zoom >= 1.0f && m_zoom <= 45.0f)
        m_zoom -= yoffset;
    if (m_zoom <= 1.0f)
        m_zoom = 1.0f;
    if (m_zoom >= 45.0f)
        m_zoom = 45.0f;
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::UpdateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    
    // Re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // Normalize the vectors
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::test()
{
    std::cout << "\nRunning Camera tests...\n";

    // Test camera initialization
    Camera camera;
    assert(camera.GetPosition() == glm::vec3(0.0f, 0.0f, 3.0f) && "Initial position incorrect");
    assert(camera.GetFront() == glm::vec3(0.0f, 0.0f, -1.0f) && "Initial front vector incorrect");
    assert(camera.GetUp() == glm::vec3(0.0f, 1.0f, 0.0f) && "Initial up vector incorrect");
    assert(camera.GetZoom() == 45.0f && "Initial zoom incorrect");

    // Test camera movement
    camera.ProcessKeyboard(Camera::CameraMovement::FORWARD, 1.0f);
    assert(camera.GetPosition().z < 3.0f && "Forward movement failed");

    camera.ProcessKeyboard(Camera::CameraMovement::RIGHT, 1.0f);
    assert(camera.GetPosition().x > 0.0f && "Right movement failed");

    // Test mouse movement
    camera.ProcessMouseMovement(10.0f, 0.0f);
    assert(camera.GetFront() != glm::vec3(0.0f, 0.0f, -1.0f) && "Mouse movement failed to update front vector");

    // Test mouse scroll
    float initialZoom = camera.GetZoom();
    camera.ProcessMouseScroll(1.0f);
    assert(camera.GetZoom() < initialZoom && "Mouse scroll failed to update zoom");

    std::cout << "Camera tests passed!\n";
}
