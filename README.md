# vrr-test

## Project Overview

This project (`vrr-test`) is a minimal C++/OpenGL application designed for visually testing Variable Refresh Rate (VRR) on a display.
It renders a solid white vertical bar that moves smoothly across the screen, looping continuously.
This simple visual is intended to test VRR capability of your setup.

## Dependencies

This project relies on the following:

*   **C++23 Compliant Compiler**: The project is written using C++23 features.
*   **CMake**: Used as the build system (version 3.10.0 or higher recommended).
*   **OpenGL**: The core graphics API used for rendering.
*   **GLEW**: The OpenGL Extension Wrangler Library, used for managing OpenGL extensions.
*   **GLFW**: A multi-platform library for creating windows with OpenGL contexts and managing input.
*   **GLM**: OpenGL Mathematics, a header-only C++ mathematics library for graphics software.

These dependencies are typically handled by CMake during the configuration step. Ensure you have them installed on your system or that CMake can find them.

## How to Build

This project uses CMake for its build system. Follow these steps to build the application:

1.  **Create a build directory:**
    ```bash
    mkdir build
    cd build
    ```
2.  **Configure CMake:**
    ```bash
    cmake ..
    ```

3.  **Build the project:**
    ```bash
    cmake --build .
    ```

4.  **Run the application:**
    ```bash
    ./build/src/vrr-test
    ```
    
## Controls
    
-   `Esc`, `Q`: Quit the application.
-   `V`: Toggle VSync.
-   `W`, `S`: Increase/decrease the speed of the moving bar.
-   `E`, `D`: Increase/decrease the FPS limit by 10.
-   `R`, `F`: Increase/decrease the FPS limit by 1.

