#include "DataManager.h"
#include <fstream>
#include <iostream>
#include <cassert>

DataManager::DataManager(const std::string& filename)
    : m_filename(filename)
{
}

bool DataManager::Initialize()
{
    m_filename = "snapengine_data.json";
    std::cout << "DataManager initialized with file: " << m_filename << std::endl;
    return true;
}

bool DataManager::LoadData()
{
    std::cout << "Attempting to load data from: " << m_filename << std::endl;
    std::ifstream file(m_filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << m_filename << "\n";
        return false;
    }

    nlohmann::json j;
    try
    {
        file >> j;
        std::cout << "Successfully parsed JSON data" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        return false;
    }

    if (!j.is_array())
    {
        std::cerr << "Expected a JSON array in file: " << m_filename << "\n";
        return false;
    }

    std::cout << "Found " << j.size() << " objects in JSON array" << std::endl;

    for (auto& obj : j)
    {
        if (!obj.contains("class"))
        {
            std::cerr << "JSON object missing 'class' field; skipping...\n";
            continue;
        }

        std::string classType = obj["class"];
        std::cout << "Processing object of class: " << classType << std::endl;
        
        if (classType == "window" || classType == "Window")
        {
            std::cout << "Found window object with title: " << obj["title"] << std::endl;
            m_windowManager.addJsonObject(obj);
        }
        else
        {
            std::cerr << "Unknown class type: " << classType 
                      << " - no manager to handle this object. Skipping.\n";
        }
    }

    return true;
}

void DataManager::CreateManagedObjects()
{
    std::cout << "Creating managed objects..." << std::endl;
    // For now, we only have a WindowManager.
    // In the future, call createObjects() on other managers here.
    m_windowManager.createObjects();
}

WindowManager& DataManager::GetWindowManager()
{
    return m_windowManager;
}

void DataManager::test()
{
    std::cout << "[DataManager] Running tests...\n";

    // Create a temporary JSON file (or string) for testing.
    const char* testFilename = "test_data.json";
    {
        std::ofstream outFile(testFilename);
        outFile << R"([
            { "class": "window", "title": "Test Window 1", "width": 400, "height": 300 },
            { "class": "window", "title": "Test Window 2", "width": 800, "height": 600 },
            { "class": "unknown", "something": "ignored" }
        ])";
    }

    // Instantiate and load data.
    DataManager dm(testFilename);
    bool loadResult = dm.LoadData();
    assert(loadResult && "DataManager failed to load test_data.json");

    // We expect WindowManager to have 2 JSON objects now. (The "unknown" object is skipped.)
    const auto& windowObjects = dm.GetWindowManager().getJsonObjects();
    assert(windowObjects.size() == 2 && "WindowManager should have exactly 2 objects from test_data.json");

    // Create the actual window objects (though in a real test, we'd do more sophisticated checks)
    dm.CreateManagedObjects();

    // If we reached here with no asserts firing, the test is considered passed.
    std::cout << "[DataManager] Tests passed!\n";
}
