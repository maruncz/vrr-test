#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window
{
public:
    Window() = default;
    Window(const Window& o) = delete;
    Window(Window&& o) = delete;
    Window* operator=(const Window& o) = delete;
    Window* operator=(Window&& o) = delete;
    virtual ~Window();

    void init();

    void exec();

private:
    void createWindow(int width, int height, const char *title,
                      GLFWmonitor *monitor, GLFWwindow *share);
    void onkeyboard(GLFWwindow *window, int key, int scancode,
                    int action, int mods);
    void initGL();
    void processKeyboardInput();
    static void onerror(int error, const char *description);
    static void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id,
                                       GLenum severity, GLsizei length,
                                       const GLchar *message,
                                       const void *userParam);

    int win_width = 1600, win_height = 900;
    GLfloat win_aspect   = 1600.0f / 900.0f;
    GLFWwindow *window   = nullptr;
    glm::ivec2 frameBufferSize{0, 0};

};

#endif // WINDOW_HPP
