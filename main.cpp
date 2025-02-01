#include <iostream>
#include <string>
#include <nlohmann/json.hpp> // Include nlohmann/json if not already

#include "Window.h"
#include "ManagerBase.h"
#include "WindowManager.h"
#include "DataManager.h"
#include "Model.h"
#include "D3D11Renderer.h" 
#include <DirectXMath.h>
#include <chrono>

using namespace DirectX;
using Clock = std::chrono::high_resolution_clock;

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
        std::cout << "Running SnapEngine tests...\n";

        // Test the Window class
        Window::test();

        // Test the WindowManager
        WindowManager::test();

        // Test the DataManager
        DataManager::test();

        // Test the Model class
        Model::test();

        // Test the Renderer classes
        RendererBase::test();
        D3D11Renderer::test();

        std::cout << "All tests done.\n";
    }
    else
    {
        std::cout << "Welcome to SnapEngine!\n";

        // Create a DataManager with the JSON file we want to load
        DataManager dataManager("snapengine_data.json");
        if (!dataManager.LoadData())
        {
            std::cerr << "Failed to load data from snapengine_data.json\n";
            return -1;
        }

        // Create all objects (e.g., windows)
        try
        {
            dataManager.CreateManagedObjects();
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to create objects: " << e.what() << std::endl;
            return -1;
        }

        // Get the WindowManager from the DataManager
        WindowManager& windowManager = dataManager.GetWindowManager();
        auto& windows = windowManager.GetWindows();

        if (windows.empty())
        {
            std::cerr << "No windows created; cannot continue.\n";
            return -1;
        }

        // We'll use the first window for rendering
        Window* mainWindow = windows.front().get();
        if (!mainWindow)
        {
            std::cerr << "Window pointer is null; cannot continue.\n";
            return -1;
        }

        // Create and initialize a D3D11Renderer
        D3D11Renderer renderer;
        if (!renderer.Initialize(static_cast<void*>(mainWindow->GetHandle())))
        {
            std::cerr << "Failed to initialize the D3D11Renderer.\n";
            return -1;
        }

        // Load a 3D model via the Model class
        Model myModel;
        if (!myModel.LoadFromFile("test_assets/VibrantKnight/VibrantKnight.obj"))
        {
            std::cerr << "Failed to load 'test_assets/VibrantKnight/VibrantKnight.obj'.\n";
            return -1;
        }
        else
        {
            std::cout << "Successfully loaded 'test_assets/VibrantKnight/VibrantKnight.obj'!\n";
            std::cout << "Model contains " << myModel.GetMeshCount() << " meshes.\n";

            for (size_t i = 0; i < myModel.GetMeshCount(); ++i)
            {
                const auto& mesh = myModel.GetMeshes()[i];
                std::cout << "Mesh " << i << ": " << mesh.vertices.size() / 8 << " vertices, "
                          << mesh.indices.size() << " indices.\n";
                std::cout << "Mesh " << i << " first vertex: "
                        << mesh.vertices[0] << ", " << mesh.vertices[1] << ", " << mesh.vertices[2] << "\n";
            }
        }

        // Initialize rotation angle
        float rotationAngle = 3.90f;

        // Main loop
        bool running = true;
        //float rotationSpeed = XM_PI / 2.0f; // Rotate 90 degrees per second
        auto lastTime = Clock::now();

        while (running)
        {
            auto currentTime = Clock::now();
            std::chrono::duration<float> deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            // Process window messages
            for (auto& wPtr : windows)
            {
                if (!wPtr->ProcessMessages())
                {
                    running = false;
                    break;
                }
            }

   //         // Increment rotation angle based on delta time
   //         rotationAngle += rotationSpeed * deltaTime.count();
			//if (rotationAngle > XM_2PI)
			//	rotationAngle -= XM_2PI;

            // Update world matrix to rotate on both Y and X axes
            XMMATRIX world = XMMatrixRotationY(rotationAngle); // Rotate around Y-axis

            XMMATRIX view = XMMatrixLookAtLH(
                XMVectorSet(0.0f, 2.0f, -5.0f, 1.0f), // Camera position
                XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),  // Look-at point
                XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)   // Up vector
            );
            XMMATRIX proj = XMMatrixPerspectiveFovLH(
                XM_PIDIV4,   // Field of view (45 degrees)
                1280.0f / 720.0f, // Aspect ratio
                0.1f,        // Near plane
                100.0f       // Far plane
            );
            XMMATRIX worldViewProj = XMMatrixTranspose(world * view * proj);
            
            renderer.UpdateConstantBuffer(worldViewProj);

            // Begin rendering
            renderer.BeginFrame();

            // Draw the loaded model
            if (myModel.GetMeshCount() > 0)
            {
                renderer.DrawModel(myModel);
            }
            else
            {
                std::cerr << "Model has no meshes to draw.\n";
            }

            // End rendering
            renderer.EndFrame();
        }

        std::cout << "Exiting SnapEngine.\n";
    }

    return 0;
}
