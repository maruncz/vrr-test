# vrr-test

## Project Overview

This project (`vrr-test`) is a minimal C++/OpenGL application designed for visually testing Variable Refresh Rate (VRR) on a display.
It renders a solid white vertical bar that moves smoothly across the screen, looping continuously.
This simple visual is intended to test VRR capability of your setup.

## Dependencies

This project relies on the following external libraries:

*   **GLEW**: The OpenGL Extension Wrangler Library, used for managing OpenGL extensions.
*   **GLFW**: A multi-platform library for creating windows with OpenGL contexts and managing input.
*   **GLM**: OpenGL Mathematics, a header-only C++ mathematics library for graphics software.

These dependencies are typically handled by CMake during the configuration step. Ensure you have them installed on your system or that CMake can find them.

## Work in Progress

Please note that this project is currently under active development. Features may be incomplete, and bugs may be present.

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
    ./build/vrr-demo
    ```
    
