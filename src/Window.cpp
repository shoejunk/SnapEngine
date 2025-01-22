#include "Window.h"
#include <iostream>
#include <cassert>

// Win32 callback
LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		// For any unhandled messages, call the default window procedure
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}
}

// Constructor
Window::Window(const std::string& title, int width, int height)
	: m_title(title)
	, m_width(width)
	, m_height(height)
	, m_hWnd(nullptr)
	, m_isCreated(false)
{
	// Zero out the struct to avoid garbage in uninitialized fields.
	ZeroMemory(&m_windowClass, sizeof(m_windowClass));

	// Fill fields of WNDCLASSW
	m_windowClass.style = CS_HREDRAW | CS_VREDRAW;
	m_windowClass.lpfnWndProc = Window::WindowProc;
	m_windowClass.hInstance = GetModuleHandle(nullptr);
	m_windowClass.lpszClassName = L"SnapEngineWindowClass";  // Wide literal
	m_windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	m_windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
}

// Destructor
Window::~Window()
{
	// Optionally destroy the window if it still exists
	if (m_hWnd)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = nullptr;
	}
}

// Create the window
bool Window::Create()
{
	// Register the window class as UNICODE
	ATOM atom = RegisterClassW(&m_windowClass);
	if (!atom)
	{
		DWORD err = GetLastError();
		// 1410 (ERROR_CLASS_ALREADY_EXISTS) is okay if the class was already registered
		if (err != ERROR_CLASS_ALREADY_EXISTS)
		{
			std::cerr << "Failed to register window class. GetLastError()=" << err << std::endl;
			return false;
		}
	}

	// Adjust the client area to the requested dimensions
	RECT r = { 0, 0, m_width, m_height };
	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
	int adjustedWidth = r.right - r.left;
	int adjustedHeight = r.bottom - r.top;

	// Convert our ASCII std::string title to a std::wstring
	std::wstring wideTitle(m_title.begin(), m_title.end());

	// Create the window using the wide API
	m_hWnd = CreateWindowExW(
		0,
		m_windowClass.lpszClassName,  // class name
		wideTitle.c_str(),            // title
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		adjustedWidth, adjustedHeight,
		nullptr,    // parent
		nullptr,    // menu
		m_windowClass.hInstance,
		nullptr
	);

	if (!m_hWnd)
	{
		DWORD err = GetLastError();
		std::cerr << "Failed to create window. GetLastError()=" << err << std::endl;
		return false;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	m_isCreated = true;
	return true;
}

// Pump Win32 messages
bool Window::ProcessMessages()
{
	if (!m_isCreated) return false;

	MSG msg = {};
	// PeekMessageW (rather than PeekMessageA) when using Unicode
	while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false; // Window is closing
		}

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return true;
}

// Simple test routine
void Window::test()
{
	std::cout << "[Window] Running tests...\n";

	Window testWin("Test Window", 400, 300);
	bool created = testWin.Create();
	assert(created && "Failed to create test window.");

	// Process messages for a few frames
	int frames = 50;
	while (frames-- > 0)
	{
		if (!testWin.ProcessMessages())
		{
			break;
		}
		// Sleep a bit to simulate some �engine ticks�
		Sleep(10);
	}

	std::cout << "[Window] Tests passed!\n";
}
