#pragma once

#include <string>
#include <Windows.h>

class Window
{
public:
	Window(const std::string& title, int width, int height);
	~Window();

	bool Create();
	bool ProcessMessages();

	/// Test method to verify window creation
	static void test();

private:
	/// Window procedure for handling Win32 messages.
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Fields
	std::string m_title;  // ASCII title that we'll convert to wide
	int m_width;
	int m_height;

	HWND m_hWnd;
	bool m_isCreated;

	/// Use the wide struct for registering a window class in Unicode mode.
	WNDCLASSW m_windowClass;
};
