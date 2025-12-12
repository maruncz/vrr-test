#include "window.hpp"
#include "glmisc.hpp"
#include <cmath>
#include <cstdio>
#include <format>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <numbers>
#include <print>
#include <source_location>
#include <stdexcept>
#include <thread>
#include "misc.hpp"

Window::~Window()
{
    if (window != nullptr)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    glfwTerminate();
}

void Window::init()
{
    if (glfwInit() == 0)
    {
        throw std::runtime_error(std::format("{:short}: cannot initialize glfw", std::source_location::current()));
    }
    createWindow(win_width, win_height, "vrr-test", nullptr,
                 nullptr);
    initGL();

    glfwSetFramebufferSizeCallback(window, onFramebufferSize);
    glfwGetFramebufferSize(window, &frameBufferSize.x, &frameBufferSize.y);
    glViewport(0, 0, frameBufferSize.x, frameBufferSize.y);

    glfwSwapInterval(vsync);
}

void Window::exec()
{
    lastFrameTime = std::chrono::steady_clock::now();
    while (glfwWindowShouldClose(window) == 0)
    {
        update_fps_counter(get_frametime());
        const auto waitUntil = lastFrameTime
                             + std::chrono::microseconds(static_cast<unsigned>(
                                 1'000'000.0 / fpsLimit));

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT);
        GLMisc::checkGLerror();

        strip.draw(calcPos());

        std::this_thread::sleep_until(waitUntil);
        lastFrameTime = waitUntil;

        glfwSwapBuffers(window);
        GLMisc::checkGLerror();
        glfwPollEvents();
        GLMisc::checkGLerror();
    }
}

void Window::createWindow(int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share)
{

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GL_TRUE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
    glfwSetErrorCallback(Window::onerror);

    window     = glfwCreateWindow(width, height, title, monitor, share);
    win_aspect = float(win_width) / float(win_height);

    if (window == nullptr)
    {
        glfwTerminate();
        const auto loc = std::source_location::current();
        throw std::runtime_error(std::format("{}:{}:{}: error opening window", loc.file_name(), loc.function_name(), loc.line()));
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    auto func = [](GLFWwindow *window, int key, int scancode, int action,
                   int mods) {
        static_cast<Window *>(glfwGetWindowUserPointer(window))
        ->onkeyboard(window, key, scancode, action, mods);
    };
    glfwSetKeyCallback(window, func);

    GLMisc::checkGLerror();
}

void Window::onkeyboard(GLFWwindow* window, int key,
                        [[maybe_unused]] int scancode, int action,
                        [[maybe_unused]] int mods)
{
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_V && action == GLFW_PRESS)
    {
        vsync = (vsync == 0) ? 1 : 0;
        glfwSwapInterval(vsync);
        std::println("swap interval {}", vsync);
        std::cout.flush();
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        speed *= speedStep;
        std::println("speed {}", speed);
        std::cout.flush();
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        speed /= speedStep;
        std::println("speed {}", speed);
        std::cout.flush();
    }

    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        fpsLimit += 10;
        std::println("fps limit {}", fpsLimit);
        std::cout.flush();
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        fpsLimit -= 10;
        std::println("fps limit {}", fpsLimit);
        std::cout.flush();
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        fpsLimit += 1;
        std::println("fps limit {}", fpsLimit);
        std::cout.flush();
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        fpsLimit -= 1;
        std::println("fps limit {}", fpsLimit);
        std::cout.flush();
    }
}

void Window::initGL()
{
    glewInit();

#ifndef NDEBUG
    GLint flags = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) != 0)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(Window::glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                              nullptr, GL_TRUE);
        GLMisc::checkGLerror();
    }
#endif

    std::println("{}\n{}",
                 reinterpret_cast<const char*>(glGetString(GL_RENDERER)),
                 reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    std::cout.flush();
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.0F, 0.0F, 0.0F, 0.0F);

    GLMisc::checkGLerror();
}

void Window::onerror(int error, const char *description)
{
    throw std::runtime_error(std::format("Window::onerror: {}: {}", error, description));
}

void Window::glDebugOutput(GLenum source, GLenum type, GLuint id,
                           GLenum severity, [[maybe_unused]] GLsizei length,
                           const GLchar* message,
                           [[maybe_unused]] const void* userParam)
{
    std::println("---------------\nDebug message ({}): {}",id, message);
    switch (source)
    {
    case GL_DEBUG_SOURCE_API: std::puts("Source: API"); break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::puts("Source: Window System");
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::puts("Source: Shader Compiler");
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::puts("Source: Third Party");
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::puts("Source: Application");
        break;
    case GL_DEBUG_SOURCE_OTHER: std::puts("Source: Other"); break;
    default: std::println("Source: Unknown ({})", source); break;
    }
    std::putchar('\n');

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR: std::puts("Type: Error"); break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::puts("Type: Deprecated Behaviour");
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::puts("Type: Undefined Behaviour");
        break;
    case GL_DEBUG_TYPE_PORTABILITY: std::puts("Type: Portability"); break;
    case GL_DEBUG_TYPE_PERFORMANCE: std::puts("Type: Performance"); break;
    case GL_DEBUG_TYPE_MARKER: std::puts("Type: Marker"); break;
    case GL_DEBUG_TYPE_PUSH_GROUP: std::puts("Type: Push Group"); break;
    case GL_DEBUG_TYPE_POP_GROUP: std::puts("Type: Pop Group"); break;
    case GL_DEBUG_TYPE_OTHER: std::puts("Type: Other"); break;
    default: std::println("Type: Unknown ({})", type); break;
    }
    std::putchar('\n');

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH: std::puts("Severity: high"); break;
    case GL_DEBUG_SEVERITY_MEDIUM: std::puts("Severity: medium"); break;
    case GL_DEBUG_SEVERITY_LOW: std::puts("Severity: low"); break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::puts("Severity: notification");
        break;
    default: std::println("Severity: Unknown ({})", severity); break;
    }
    std::putchar('\n');
    std::putchar('\n');
}

void Window::onFramebufferSize(GLFWwindow* window, int width, int height)
{
    auto* w = static_cast<Window*>(glfwGetWindowUserPointer(window));
    w->frameBufferSize.x = width;
    w->frameBufferSize.y = height;
    glViewport(0, 0, width, height);
}

double Window::calcPos() const
{
    static auto lastTime  = glfwGetTime();
    static double phase   = 0.0;
    const auto time = glfwGetTime();
    phase                += 2 * std::numbers::pi * speed * (time - lastTime);
    lastTime              = time;
    return std::sin(phase);
}

void Window::update_fps_counter(double frametime)
{
    static int frame_count;
    static double time  = 0;
    time               += frametime;
    if (time > 1)
    {
        const auto fps = frame_count / time;
        std::println("fps: {:>6.2f}", fps);
        std::cout.flush();
        frame_count = 0;
        time        = 0;
    }
    ++frame_count;
}

double Window::get_frametime()
{
    static auto pervious_seconds = glfwGetTime();
    const auto current_seconds   = glfwGetTime();
    auto lastFrameTime           = current_seconds - pervious_seconds;
    pervious_seconds             = current_seconds;
    return lastFrameTime;
}
