#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "strip.hpp"
#include <chrono>
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
    void onkeyboard(GLFWwindow* window, int key, int scancode, int action,
                    int mods);
    static void initGL();
    static void onerror(int error, const char *description);
    static void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id,
                                       GLenum severity, GLsizei length,
                                       const GLchar *message,
                                       const void *userParam);
    static void onFramebufferSize(GLFWwindow* window, int width, int height);
    [[nodiscard]] double calcPos() const;

    static void update_fps_counter(double frametime);
    static double get_frametime();

    int win_width        = 1600;
    int win_height       = 900;
    GLfloat win_aspect   = 1600.0F / 900.0F;
    GLFWwindow *window   = nullptr;
    glm::ivec2 frameBufferSize{0, 0};

    Strip strip;

    static constexpr float speedStep {1.3f};
    float speed {0.1F};
    int vsync         = 1;
    unsigned fpsLimit = 200;
    std::chrono::time_point<std::chrono::steady_clock> lastFrameTime;
};

#endif // WINDOW_HPP
