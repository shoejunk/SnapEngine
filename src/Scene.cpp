#include "Scene.h"
#include <iostream>
#include <cassert>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// Initialize static members
bool Scene::s_testMode = false;

Scene::Scene()
    : m_camera(std::make_unique<Camera>())
    , m_shader(std::make_unique<Shader>("shaders/basic.vert", "shaders/basic.frag"))
    , m_firstMouse(true)
    , m_lastX(0.0)
    , m_lastY(0.0)
{
    if (!s_testMode)
    {
        m_shader->Use();
        m_shader->SetMat4("projection", glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f));
    }
}

Scene::~Scene()
{
}

void Scene::Update(float deltaTime)
{
    if (s_testMode)
        return;

    // Update any scene logic here
}

void Scene::Render()
{
    if (s_testMode)
        return;

    // Clear buffers
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use shader and set common uniforms
    m_shader->Use();
    m_shader->SetMat4("view", m_camera->GetViewMatrix());

    // Render each object
    for (const auto& obj : m_objects)
    {
        // Calculate model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, obj.position);
        model = glm::scale(model, obj.scale);
        model = glm::rotate(model, glm::radians(obj.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(obj.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(obj.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        m_shader->SetMat4("model", model);
        obj.model->Draw();
    }
}

void Scene::AddModel(std::shared_ptr<Model> model, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation)
{
    SceneObject obj;
    obj.model = model;
    obj.position = position;
    obj.scale = scale;
    obj.rotation = rotation;
    m_objects.push_back(obj);
}

void Scene::OnKeyInput(int key, int scancode, int action, int mods)
{
    if (s_testMode)
        return;

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            switch (key)
            {
            case GLFW_KEY_W:
                m_camera->ProcessKeyboard(Camera::FORWARD, 0.1f);
                break;
            case GLFW_KEY_S:
                m_camera->ProcessKeyboard(Camera::BACKWARD, 0.1f);
                break;
            case GLFW_KEY_A:
                m_camera->ProcessKeyboard(Camera::LEFT, 0.1f);
                break;
            case GLFW_KEY_D:
                m_camera->ProcessKeyboard(Camera::RIGHT, 0.1f);
                break;
            }
        }
    }
}

void Scene::OnMouseMove(double xpos, double ypos)
{
    if (s_testMode)
        return;

    if (m_firstMouse)
    {
        m_lastX = xpos;
        m_lastY = ypos;
        m_firstMouse = false;
    }

    float xoffset = static_cast<float>(xpos - m_lastX);
    float yoffset = static_cast<float>(m_lastY - ypos); // Reversed since y-coordinates range from bottom to top

    m_lastX = xpos;
    m_lastY = ypos;

    m_camera->ProcessMouseMovement(xoffset, yoffset);
}

void Scene::OnMouseScroll(double xoffset, double yoffset)
{
    if (s_testMode)
        return;

    m_camera->ProcessMouseScroll(static_cast<float>(yoffset));
}

void Scene::test()
{
    std::cout << "\nRunning Scene tests...\n";

    // Enable test mode for this test
    SetTestMode(true);

    // Test scene creation
    Scene scene;
    assert(scene.GetCamera() != nullptr && "Camera not created");
    assert(scene.GetModels().empty() && "Scene should start empty");

    // Test model addition
    auto model = std::make_shared<Model>();
    scene.AddModel(model);
    assert(scene.GetModels().size() == 1 && "Failed to add model to scene");
    assert(scene.GetModels()[0].model == model && "Wrong model in scene");

    // Test model transform
    glm::vec3 position(1.0f, 2.0f, 3.0f);
    glm::vec3 scale(2.0f);
    glm::vec3 rotation(45.0f, 90.0f, 180.0f);
    scene.AddModel(model, position, scale, rotation);
    
    const auto& obj = scene.GetModels().back();
    assert(obj.position == position && "Wrong position");
    assert(obj.scale == scale && "Wrong scale");
    assert(obj.rotation == rotation && "Wrong rotation");

    // Disable test mode
    SetTestMode(false);

    std::cout << "Scene tests passed!\n";
}
