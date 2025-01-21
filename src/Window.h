#pragma once

#include <string>
#include <windows.h> // Win32 API

/// \class Window
/// \brief Represents a single Win32 window in SnapEngine.
///
/// Multiple instances of this class can be created to show multiple
/// windows at once. It provides basic window creation and message
/// handling using the Win32 API.
class Window
{
public:
	/// \brief Constructs a new Window object.
	///
	/// \param title  The title for the window (displayed in the title bar).
	/// \param width  The desired width of the client area in pixels.
	/// \param height The desired height of the client area in pixels.
	Window(const std::string& title, int width, int height);

	/// \brief Destroys the window and unregisters the window class if needed.
	~Window();

	/// \brief Creates and shows the Win32 window.
	/// \return True if the window was successfully created, otherwise false.
	bool Create();

	/// \brief Processes the window messages (e.g., close, resize).
	/// 
	/// \return True if the window should continue running, false if it's closed.
	bool ProcessMessages();

	/// \brief Gets the handle to the Win32 window (HWND).
	/// \return A Win32 HWND handle.
	HWND GetHandle() const { return m_hWnd; }

	/// \brief Unit test for the Window class.
	/// 
	/// Creates and shows a test window, processes a few messages, then closes it.
	static void test();

private:
	/// Win32 procedure callback for handling window events.
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	std::string m_title;  ///< The window title
	int m_width;          ///< The width of the client area
	int m_height;         ///< The height of the client area
	HWND m_hWnd;          ///< Win32 window handle
	WNDCLASS m_windowClass; ///< Win32 window class structure

	bool m_isCreated;     ///< Tracks whether the window has been successfully created
};
