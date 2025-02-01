#pragma once

#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>

#include "ManagerBase.h"
#include "Window.h"

/// \class WindowManager
/// \brief Manages the creation of Window instances from JSON data.
/// 
/// The JSON data for each window should look like:
/// {
///     "title": "My Window",
///     "width": 800,
///     "height": 600
/// }
class WindowManager : public ManagerBase
{
public:
    /// \brief Default constructor
    WindowManager() = default;
    
    /// \brief Default destructor
    ~WindowManager() = default;

    /// \brief Initializes the window manager.
    ///
    /// Creates a default window if no windows have been created yet.
    /// \return true if initialization was successful, false otherwise.
    bool Initialize();

    /// \brief Creates all Window objects from the stored JSON data.
    ///
    /// Expects each JSON object to have the keys "title", "width", and "height".
    /// Throws std::runtime_error if a key is missing.
    void createObjects() override;

    /// \brief Process messages for all windows.
    ///
    /// \return true if all windows are still open, false if any window should close.
    bool ProcessMessages();

    /// \brief Runs the test suite for the WindowManager class.
    ///
    /// Creates a few windows via JSON data, ensures they are created properly.
    static void test();

    /// \brief Access all created Window instances.
    ///
    /// \return A const reference to the list of managed windows.
    const std::vector<std::unique_ptr<Window>>& GetWindows() const { return m_windows; }

private:
    std::vector<std::unique_ptr<Window>> m_windows; ///< Stores the created Window objects.
};
