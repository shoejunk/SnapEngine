#include "WindowManager.h"
#include <iostream>
#include <cassert>

void WindowManager::createObjects()
{
    // Clear any previously created windows.
    m_windows.clear();

    // Get the JSON objects stored in ManagerBase
    const auto& objects = getJsonObjects();

    for (const auto& obj : objects)
    {
        // Validate required fields
        if (!obj.contains("title") || !obj.contains("width") || !obj.contains("height"))
        {
            throw std::runtime_error(
                "WindowManager::createObjects - Missing required field(s) in JSON object."
            );
        }

        // Extract data
        std::string title   = obj.at("title").get<std::string>();
        int         width   = obj.at("width").get<int>();
        int         height  = obj.at("height").get<int>();

        // Create a new Window instance
        auto window = std::make_unique<Window>(title, width, height);

        // Attempt to create (open) the window
        if (!window->Create())
        {
            throw std::runtime_error(
                "WindowManager::createObjects - Failed to create window: " + title
            );
        }

        // Store the newly created window
        m_windows.push_back(std::move(window));
    }
}

void WindowManager::test()
{
    std::cout << "[WindowManager] Running tests...\n";

    // We will create a WindowManager instance to test
    WindowManager manager;

    // Prepare a few JSON objects
    nlohmann::json window1 = {
        {"title", "Test Window 1"},
        {"width", 640},
        {"height", 480}
    };

    nlohmann::json window2 = {
        {"title", "Test Window 2"},
        {"width", 800},
        {"height", 600}
    };

    // Add them to the manager
    manager.addJsonObject(window1);
    manager.addJsonObject(window2);

    // Create the windows
    manager.createObjects();

    // Verify we actually created windows
    const auto& windows = manager.GetWindows();
    assert(windows.size() == 2 && "WindowManager should have created 2 windows.");

    std::cout << "[WindowManager] Tests passed!\n";
}
