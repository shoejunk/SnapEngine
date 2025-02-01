#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "WindowManager.h"

/// \class DataManager
/// \brief Loads JSON data from a file and distributes objects to the appropriate managers.
///
/// The data file should contain a JSON array of objects. Each object must have a "class" 
/// field that indicates which manager should handle it (e.g., "window"). Currently, 
/// DataManager routes "window" objects to WindowManager. Extend as needed for other managers.
class DataManager
{
public:
    /// \brief Default constructor
    DataManager() = default;

    /// \brief Constructs a DataManager with the given filename.
    /// \param filename The path to the JSON file containing the array of objects.
    explicit DataManager(const std::string& filename);

    /// \brief Initialize the DataManager with default settings
    /// \return True if initialization was successful, false otherwise
    bool Initialize();

    /// \brief Loads and parses the JSON file, distributing objects to the appropriate manager(s).
    ///
    /// \return True if the file was successfully loaded and parsed, false otherwise.
    bool LoadData();

    /// \brief Instructs all owned managers to create their objects.
    ///
    /// This calls createObjects() on each managed manager so that
    /// the actual objects can be instantiated.
    void CreateManagedObjects();

    /// \brief Provides access to the internal WindowManager.
    /// \return A reference to the WindowManager instance.
    WindowManager& GetWindowManager();

    /// \brief Runs basic unit tests for the DataManager class.
    ///
    /// Attempts to load a small JSON sample, verifies distribution of objects, etc.
    static void test();

private:
    /// \brief The name/path of the JSON file to load.
    std::string m_filename;

    /// \brief The WindowManager instance owned by this DataManager.
    ///
    /// Extend this class to own other manager types as needed.
    WindowManager m_windowManager;
};
