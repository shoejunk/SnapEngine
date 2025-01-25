### SnapEngine Development Guidelines

#### **Guidelines**
This guide outlines the structure, development, documentation, and testing practices for SnapEngine, a C++20 game engine developed in Visual Studio. It also covers dependency management using CPM.cmake and API documentation practices.

1. **Project Overview**
   - Project Name: SnapEngine  
   - Language Standard: C++20  
   - IDE/Compiler: Visual Studio (MSVC)  
   - Dependency Manager: CPM.cmake  
   - Goal: A modular, testable, and well-documented game engine.  

2. **Repository Structure**
   ```
   SnapEngine/
   ├─ cmake/
   │   └─ cpm.cmake           # Dependency manager
   ├─ src/
   │   ├─ ClassName.h
   │   ├─ ClassName.cpp
   │   ├─ (One header and source file per class)
   ├─ main.cpp                # Entry point
   ├─ CMakeLists.txt
   ├─ SnapEngine.sln          # Visual Studio solution
   └─ Development_Guidelines.txt
   ```
   - Follow a "one .h and .cpp per class" convention.  
   - Use `main.cpp` as the entry point and test harness.  

3. **Build System**
   - Include `cpm.cmake` in your project:  
     ```cpp
     include(cmake/cpm.cmake)
     CPMAddPackage(NAME SomeLibrary GITHUB_REPOSITORY <user>/<repo> VERSION <version>)
     add_executable(SnapEngine main.cpp)
     target_link_libraries(SnapEngine PRIVATE SomeLibrary)
     ```  

4. **Source Code Conventions**
   - Classes & Methods: PascalCase (e.g., `WindowManager`, `Initialize()`).  
   - Private Members: `m_` prefix (e.g., `m_windowWidth`).  
   - Documentation: Use Doxygen-style comments for public APIs.

5. **Testing**
   - Each class includes a static `test()` function for unit testing using `<cassert>`.  

6. **Main Entry Point**
   - The `main()` function supports:
     - Command-Line Arguments: `--test` runs tests.  
     - Game Execution: Initializes and runs the engine.  

7. **Collaboration**
   - Version Control: Use Git with a feature-branch workflow.  
   - CI/CD: Automate builds and tests on commits and pull requests.  

Happy Coding!  
SnapEngine Team
