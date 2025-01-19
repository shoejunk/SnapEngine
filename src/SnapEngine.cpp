// SnapEngine.cpp
#include "SnapEngine/SnapEngine.hpp"

SnapEngine::SnapEngine() {
    std::cout << "SnapEngine created." << std::endl;
}

SnapEngine::~SnapEngine() {
    std::cout << "SnapEngine destroyed." << std::endl;
}

void SnapEngine::initialize() {
    std::cout << "SnapEngine initialized." << std::endl;
}

void SnapEngine::run() {
    std::cout << "SnapEngine is running!" << std::endl;
}
