#pragma once

#include <string>
#include <Windows.h>

/**
 * \class Window
 * \brief Encapsulates a Win32 window creation and event handling.
 */
class Window
{
public:
    /**
     * \brief Constructs a Window object with a given title and dimensions.
     * \param title ASCII string for the window title (will be converted to wide string).
     * \param width Desired window width (client area).
     * \param height Desired window height (client area).
     */
    Window(const std::string& title, int width, int height);

    /// \brief Destructor. Cleans up any resources (e.g., unregister window class if needed).
    ~Window();

    /**
     * \brief Creates the actual Win32 window and displays it.
     * \return True if creation succeeded, false otherwise.
     */
    bool Create();

    /**
     * \brief Processes Win32 messages (input, close, etc.).
     * \return False if the window received a close event, true otherwise.
     */
    bool ProcessMessages();

    /**
     * \brief Retrieves the native Win32 handle (HWND) of this window.
     * \return The HWND for this window.
     */
    HWND GetHandle() const { return m_hWnd; }

    /**
     * \brief A static test method to verify window creation logic.
     */
    static void test();

private:
    /// \brief Window procedure for handling Win32 messages (static callback).
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Fields
    std::string m_title;  ///< ASCII title that we'll convert to wide for Win32 APIs.
    int m_width;
    int m_height;

    HWND m_hWnd     = nullptr;
    bool m_isCreated = false;

    /// \brief Holds data for registering a window class in Unicode mode.
    WNDCLASSW m_windowClass;
};
