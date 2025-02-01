#include "WindowManager.h"
#include <iostream>
#include <cassert>

bool WindowManager::Initialize()
{
    std::cout << "Initializing WindowManager..." << std::endl;

    // Initialize GLFW globally
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Set error callback
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW Error " << error << ": " << description << std::endl;
    });

    std::cout << "WindowManager initialized successfully" << std::endl;
    return true;
}

void WindowManager::createObjects()
{
    std::cout << "WindowManager creating objects..." << std::endl;

    // Clear any previously created windows.
    m_windows.clear();

    // Get the JSON objects stored in ManagerBase
    const auto& objects = getJsonObjects();
    std::cout << "Found " << objects.size() << " window objects to create" << std::endl;

    for (const auto& obj : objects)
    {
        try
        {
            // Validate required fields
            if (!obj.contains("title") || !obj.contains("width") || !obj.contains("height"))
            {
                throw std::runtime_error("Window object missing required fields (title, width, height)");
            }

            // Extract window properties
            std::string title = obj["title"];
            int width = obj["width"];
            int height = obj["height"];

            std::cout << "Creating window: " << title << " (" << width << "x" << height << ")" << std::endl;

            // Create the window
            auto window = std::make_unique<Window>(title, width, height);
            if (!window->Create())
            {
                throw std::runtime_error("Failed to create window: " + title);
            }

            std::cout << "Successfully created window: " << title << std::endl;

            // Add to our collection
            m_windows.push_back(std::move(window));
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error creating window from JSON: " << e.what() << std::endl;
        }
    }

    // If no windows were created from JSON, create a default window
    if (m_windows.empty())
    {
        std::cerr << "No windows created from JSON, creating default window" << std::endl;
        auto window = std::make_unique<Window>("SnapEngine", 1280, 720);
        if (!window->Create())
        {
            throw std::runtime_error("Failed to create default window");
        }
        m_windows.push_back(std::move(window));
    }
}

bool WindowManager::ProcessMessages()
{
    bool allWindowsOpen = true;

    // Process messages for all windows
    for (auto& window : m_windows)
    {
        if (!window->ProcessMessages())
        {
            allWindowsOpen = false;
        }
    }

    return allWindowsOpen;
}

void WindowManager::test()
{
    std::cout << "[WindowManager] Running tests...\n";

    // Create a test instance
    WindowManager manager;

    // Test JSON parsing
    nlohmann::json testJson = {
        {"title", "Test Window"},
        {"width", 800},
        {"height", 600}
    };

    // Add test object
    manager.addJsonObject(testJson);

    // Initialize
    assert(manager.Initialize() && "WindowManager initialization failed");

    // Create objects from JSON
    manager.createObjects();

    // Verify window was created
    assert(!manager.GetWindows().empty() && "No windows created");
    assert(manager.GetWindows()[0]->GetWidth() == 800 && "Incorrect window width");
    assert(manager.GetWindows()[0]->GetHeight() == 600 && "Incorrect window height");

    // Process a few frames
    for (int i = 0; i < 10; ++i)
    {
        if (!manager.ProcessMessages())
        {
            break;
        }
    }

    std::cout << "[WindowManager] Tests passed!\n";
}
