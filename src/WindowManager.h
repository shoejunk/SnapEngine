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
    WindowManager() : m_testMode(false) {}
    
    /// \brief Default destructor
    ~WindowManager() = default;

    /// \brief Initializes the window manager.
    /// \return True if initialization was successful.
    bool Initialize();

    /// \brief Creates window objects from JSON data.
    void createObjects() override;

    /// \brief Process window messages.
    /// \return True if all windows are still open.
    bool ProcessMessages();

    /// \brief Get the list of windows.
    /// \return Vector of window pointers.
    const std::vector<std::unique_ptr<Window>>& GetWindows() const { return m_windows; }

    /// \brief Enable or disable test mode.
    /// In test mode, no actual GLFW windows are created.
    /// \param enabled True to enable test mode.
    void SetTestMode(bool enabled) { m_testMode = enabled; }

    /// \brief Check if test mode is enabled.
    /// \return True if test mode is enabled.
    bool IsTestMode() const { return m_testMode; }

    /// \brief Run unit tests for WindowManager.
    static void test();

private:
    std::vector<std::unique_ptr<Window>> m_windows;  ///< Collection of managed windows
    bool m_testMode;                                 ///< True if running in test mode
};
