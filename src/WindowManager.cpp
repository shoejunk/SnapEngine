#include "WindowManager.h"
#include <iostream>
#include <cassert>

bool WindowManager::Initialize()
{
    std::cout << "Initializing WindowManager..." << std::endl;

    if (!m_testMode)
    {
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
    }

    std::cout << "WindowManager initialized successfully" << std::endl;
    return true;
}

void WindowManager::createObjects()
{
    std::cout << "WindowManager creating objects..." << std::endl;

    // Clear any previously created windows
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
            
            if (!m_testMode)
            {
                if (!window->Create())
                {
                    throw std::runtime_error("Failed to create window: " + title);
                }
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
        
        if (!m_testMode)
        {
            if (!window->Create())
            {
                throw std::runtime_error("Failed to create default window");
            }
        }
        
        m_windows.push_back(std::move(window));
    }
}

bool WindowManager::ProcessMessages()
{
    if (m_testMode)
    {
        // In test mode, just return true to indicate windows are "open"
        return true;
    }

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

    // Enable test mode for both Window and WindowManager
    Window::SetTestMode(true);
    
    // Create a test instance
    WindowManager manager;
    manager.SetTestMode(true);

    // Test initialization
    assert(manager.Initialize() && "WindowManager initialization failed");
    assert(manager.IsTestMode() && "Test mode not enabled");

    // Test JSON parsing
    nlohmann::json testJson = {
        {"title", "Test Window"},
        {"width", 800},
        {"height", 600}
    };

    // Add test object and create windows
    manager.addJsonObject(testJson);
    manager.createObjects();

    // Verify window properties
    const auto& windows = manager.GetWindows();
    assert(!windows.empty() && "No windows created");
    assert(windows.size() == 1 && "Incorrect number of windows");
    assert(windows[0]->GetWidth() == 800 && "Incorrect window width");
    assert(windows[0]->GetHeight() == 600 && "Incorrect window height");
    assert(windows[0]->GetTitle() == "Test Window" && "Incorrect window title");

    // Test invalid JSON
    {
        WindowManager invalidManager;
        invalidManager.SetTestMode(true);
        invalidManager.Initialize();

        // Missing required fields
        nlohmann::json invalidJson = {{"title", "Invalid Window"}};
        invalidManager.addJsonObject(invalidJson);
        invalidManager.createObjects();

        // Should create default window
        const auto& invalidWindows = invalidManager.GetWindows();
        assert(invalidWindows.size() == 1 && "Should create default window");
        assert(invalidWindows[0]->GetWidth() == 1280 && "Incorrect default window width");
        assert(invalidWindows[0]->GetHeight() == 720 && "Incorrect default window height");
    }

    // Test multiple windows
    {
        WindowManager multiManager;
        multiManager.SetTestMode(true);
        multiManager.Initialize();

        nlohmann::json window1 = {
            {"title", "Window 1"},
            {"width", 800},
            {"height", 600}
        };
        nlohmann::json window2 = {
            {"title", "Window 2"},
            {"width", 1024},
            {"height", 768}
        };

        multiManager.addJsonObject(window1);
        multiManager.addJsonObject(window2);
        multiManager.createObjects();

        const auto& multiWindows = multiManager.GetWindows();
        assert(multiWindows.size() == 2 && "Incorrect number of windows");
        assert(multiWindows[0]->GetTitle() == "Window 1" && "Incorrect window 1 title");
        assert(multiWindows[1]->GetTitle() == "Window 2" && "Incorrect window 2 title");
    }

    // Test message processing in test mode
    assert(manager.ProcessMessages() && "ProcessMessages should return true in test mode");

    // Cleanup - disable test mode
    Window::SetTestMode(false);

    std::cout << "[WindowManager] Tests passed!\n";
}
