#include "Window.h"
#include <cassert>
#include <iostream>

/// Global map or storage could be used if you need to retrieve the
/// Window instance from HWND in the WindowProc. For simplicity, 
/// this example uses static members and a single WNDCLASS. 
/// For multiple windows, you can:
///   - Register multiple WNDCLASSes, or
///   - Use a map from HWND -> Window* in WindowProc.

Window::Window(const std::string& title, int width, int height)
	: m_title(title), m_width(width), m_height(height),
	m_hWnd(nullptr), m_isCreated(false)
{
	// Configure the WNDCLASS structure
	// (You can customize the window style, icon, cursor, etc. as needed)
	m_windowClass = {};
	m_windowClass.lpfnWndProc = Window::WindowProc;
	m_windowClass.hInstance = GetModuleHandle(nullptr);
	m_windowClass.lpszClassName = "SnapEngineWindowClass";
	m_windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
}

Window::~Window()
{
	// Optionally, destroy the window if it still exists
	if (m_hWnd)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = nullptr;
	}
}

bool Window::Create()
{
	// Register the window class if not already registered.
	// For multiple windows, you could register once in a static map or similar.
	if (!RegisterClass(&m_windowClass))
	{
		std::cerr << "Failed to register window class.\n";
		return false;
	}

	// Calculate the size of the window rectangle based on desired client area
	RECT rect = { 0, 0, m_width, m_height };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	int adjustedWidth = rect.right - rect.left;
	int adjustedHeight = rect.bottom - rect.top;

	m_hWnd = CreateWindow(
		m_windowClass.lpszClassName,             // Window class name
		m_title.c_str(),                         // Window title
		WS_OVERLAPPEDWINDOW,                     // Style
		CW_USEDEFAULT, CW_USEDEFAULT,            // Position
		adjustedWidth, adjustedHeight,           // Size
		nullptr,                                 // Parent
		nullptr,                                 // Menu
		m_windowClass.hInstance,                 // Instance handle
		nullptr                                  // Additional data
	);

	if (!m_hWnd)
	{
		std::cerr << "Failed to create window.\n";
		return false;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	m_isCreated = true;
	return true;
}

bool Window::ProcessMessages()
{
	if (!m_isCreated) return false;

	MSG msg = {};
	while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false; // Window is closing
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true; // Window is still active
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void Window::test()
{
	std::cout << "[Window] Running tests...\n";

	// Create a Window instance
	Window testWin("Test Window", 400, 300);
	bool created = testWin.Create();
	assert(created && "Failed to create test window.");

	// Process a few frames of messages to ensure it opens (artificial limit)
	int framesToProcess = 50;
	while (framesToProcess-- > 0)
	{
		if (!testWin.ProcessMessages())
		{
			break;
		}
		// Simulate a small sleep or game loop tick
		Sleep(10);
	}

	std::cout << "[Window] Tests passed!\n";
}
