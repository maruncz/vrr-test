#include "window.hpp"
#include <print>
#include <format>
#include <source_location>
#include <stdexcept>
#include "glmisc.hpp"



Window::~Window()
{
    if (window)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    glfwTerminate();
}

void Window::init()
{
    createWindow(win_width, win_height, "vrr-test", nullptr,
                 nullptr);
    initGL();

    glfwSetFramebufferSizeCallback(window, onFramebufferSize);
    glfwGetFramebufferSize(window, &frameBufferSize.x, &frameBufferSize.y);
    glViewport(0, 0, frameBufferSize.x, frameBufferSize.y);

    glfwSwapInterval(1);
}

void Window::exec()
{
    while (!glfwWindowShouldClose(window))
    {
        processKeyboardInput();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT);
        GLMisc::checkGLerror(HERE);

        strip.draw(0.0f);

        glfwSwapBuffers(window);
        GLMisc::checkGLerror(HERE);
        glfwPollEvents();
        GLMisc::checkGLerror(HERE);
    }
}

void Window::createWindow(int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share)
{
    if (!glfwInit())
    {
        const auto loc = std::source_location::current();
        throw std::runtime_error(std::format("{}:{}:{}: cannot initialize glfw", loc.file_name(), loc.function_name(), loc.line()));
    }
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

    if (!window)
    {
        glfwTerminate();
        const auto loc = std::source_location::current();
        throw std::runtime_error(std::format("{}:{}:{}: error opening window", loc.file_name(), loc.function_name(), loc.line()));
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
#if 0
    auto func = [](GLFWwindow *window, int key, int scancode, int action,
                   int mods) {
        static_cast<Window *>(glfwGetWindowUserPointer(window))
        ->onkeyboard(window, key, scancode, action, mods);
    };
    glfwSetKeyCallback(window, func);
#endif
}

void Window::onkeyboard(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void Window::initGL()
{
    glewInit();

#ifndef NDEBUG
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(Window::glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                              nullptr, GL_TRUE);
    }
#endif

    std::cout << glGetString(GL_RENDERER) << "\n"
              << glGetString(GL_VERSION) << std::endl;
    //std::println("{}\n{}", glGetString(GL_RENDERER), glGetString(GL_VERSION));
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLMisc::checkGLerror(HERE);
}

void Window::processKeyboardInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void Window::onerror(int error, const char *description)
{
    throw std::runtime_error(std::format("Window::onerror: {}: {}", error, description));
}

void Window::glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
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
    }
    std::putchar('\n');
    std::putchar('\n');
}

void Window::onFramebufferSize(GLFWwindow* window, int width, int height)
{
    auto w = static_cast<Window*>(glfwGetWindowUserPointer(window));
    w->frameBufferSize.x = width;
    w->frameBufferSize.y = height;
    glViewport(0, 0, width, height);
}

void Window::updatePos()
{
    const auto time = glfwGetTime();
}
