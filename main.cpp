#include <iostream>
#include <string>
#include "Window.h"
#include "ManagerBase.h"

int main(int argc, char* argv[])
{
	bool runTests = false;

	// Check for --test argument
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg == "--test")
		{
			runTests = true;
			break;
		}
	}

	if (runTests)
	{
		std::cout << "Running SnapEngine tests...\n";
		Window::test(); // Calls the static test() method inside Window class
		// ... call other test() methods from other classes if needed
		std::cout << "All tests done.\n";
	}
	else
	{
		// Normal mode: Create a window, run your engine/game loop, etc.
		std::cout << "Welcome to SnapEngine!" << std::endl;

		Window mainWindow("My SnapEngine Window", 800, 600);
		if (!mainWindow.Create())
		{
			std::cerr << "Failed to create the main window." << std::endl;
			return -1;
		}

		bool running = true;
		while (running)
		{
			running = mainWindow.ProcessMessages();
			// Add your engine update/draw calls here
		}

		std::cout << "Exiting SnapEngine." << std::endl;
	}

	return 0;
}
