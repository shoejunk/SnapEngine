#include <iostream>
#include "SnapEngine/SnapEngine.hpp"

int main() {
    std::cout << "Welcome to SnapEngine!" << std::endl;

    // Example of using SnapEngine functionality
    SnapEngine engine;
    engine.initialize();

    // Run the engine
    engine.run();

    std::cout << "Exiting SnapEngine." << std::endl;
    return 0;
}
