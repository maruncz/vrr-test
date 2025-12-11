# Project: vrr-test

## Summary

The project, named `vrr-test`, is a minimal C++/OpenGL application designed to help visually test Variable Refresh Rate (VRR) on a display. Its core function is to render a solid white vertical bar that moves smoothly from left to right, resetting its position to create a continuous loop. The simplicity of the rendering allows for a clear assessment of motion smoothness, which is the primary purpose of VRR technology.

## Architecture

The architecture is straightforward:
- **`main.cpp`**: Sets up a `Window` object.
- **`Window` class**: Uses GLFW to create a window and runs a main loop. In this loop, it continuously updates the position of a `Strip` object and calls its `draw` method.
- **`Strip` class**: Contains the vertices for a rectangle and the GLSL shader code required to render it. The vertex shader takes a 'pos' uniform to control the horizontal position of the strip on the screen.

## Relevant Files

- **`src/main.cpp`**: The main entry point of the application.
- **`src/window.hpp` / `src/window.cpp`**: Defines and implements the main application class, responsible for window creation, the main loop, and orchestrating the rendering.
- **`src/strip.hpp` / `src/strip.cpp`**: Defines and implements the class responsible for rendering the visual element (the white vertical bar).
- **`src/glshader.h` / `src/glshader.cpp`**: A helper class for compiling and managing GLSL shaders.
- **`CMakeLists.txt`**: The root CMake file, defining project name, C++ standard, and external dependencies (GLEW, glfw, glm).
