#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

#include "Window.h"
#include "ManagerBase.h"
#include "WindowManager.h"
#include "DataManager.h"
#include "Model.h"
#include "Mesh.h"
#include "Vertex.h"
#include "Tests.h"

int main(int argc, char* argv[])
{
    bool runTests = false;

    // Check for --test argument
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--test")
        {
            runTests = true;
            break;
        }
    }

    if (runTests)
    {
        return Tests::RunAllTests() ? 0 : 1;
    }

    try
    {
        std::cout << "SnapEngine starting...\n";

        // Initialize GLFW
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return 1;
        }
        std::cout << "GLFW initialized successfully\n";

        // Configure OpenGL context
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        std::cout << "OpenGL context configured\n";

        // Create window
        Window window("SnapEngine - VibrantKnight Demo", 1280, 720);
        if (!window.Create())
        {
            std::cerr << "Failed to create window" << std::endl;
            glfwTerminate();
            return 1;
        }
        std::cout << "Window created successfully\n";

        // Load VibrantKnight model
        auto model = std::make_shared<Model>();
        if (!model->LoadFromFile("test_assets/VibrantKnight/VibrantKnight.obj"))
        {
            std::cerr << "Failed to load VibrantKnight model" << std::endl;
            glfwTerminate();
            return 1;
        }
        std::cout << "VibrantKnight model loaded successfully\n";

        // Add model to scene
        Scene* scene = window.GetScene();
        scene->AddModel(model, 
            glm::vec3(0.0f, -1.0f, 0.0f),     // Position slightly below origin
            glm::vec3(0.1f, 0.1f, 0.1f),      // Scale down to reasonable size
            glm::vec3(0.0f, 180.0f, 0.0f));   // Rotate to face camera
        std::cout << "Model added to scene\n";

        std::cout << "Entering main loop...\n";
        // Main loop
        while (window.ProcessMessages())
        {
            // ProcessMessages handles everything now
        }

        std::cout << "Main loop ended\n";

        // Cleanup is handled by destructors
        glfwTerminate();
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        glfwTerminate();
        return 1;
    }
}
