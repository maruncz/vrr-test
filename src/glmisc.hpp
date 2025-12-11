#ifndef GLMISC_HPP
#define GLMISC_HPP

#include "misc.hpp"
#include <format>
#include <GL/glew.h>
#include <source_location>
#include <stdexcept>

namespace GLMisc
{

/**
 * @brief checks if opengl set error
 * @param loc location of error
 * @throw std::runtime_error with position and error string
 * @note this function does nothing in Release mode
 */
inline void checkGLerror([[maybe_unused]] const std::source_location& loc
                         = std::source_location::current())
{
#ifndef NDEBUG
    switch (glGetError())
    {
        case GL_NO_ERROR: return;
        case GL_INVALID_ENUM:
            throw std::runtime_error(std::format("{}: GL_INVALID_ENUM\n", loc));
        case GL_INVALID_VALUE:
            throw std::runtime_error(
                std::format("{}: GL_INVALID_VALUE\n", loc));
        case GL_INVALID_OPERATION:
            throw std::runtime_error(
                std::format("{}: GL_INVALID_OPERATION\n", loc));
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            throw std::runtime_error(
                std::format("{}: GL_INVALID_FRAMEBUFFER_OPERATION\n", loc));
        case GL_OUT_OF_MEMORY:
            throw std::runtime_error(
                std::format("{}: GL_OUT_OF_MEMORY\n", loc));
        case GL_STACK_UNDERFLOW:
            throw std::runtime_error(
                std::format("{}: GL_STACK_UNDERFLOW\n", loc));
        case GL_STACK_OVERFLOW:
            throw std::runtime_error(
                std::format("{}: GL_STACK_OVERFLOW\n", loc));
        default:
            throw std::runtime_error(std::format("{}: unknown error\n", loc));
    }
#endif
}

} // namespace GLMisc

#endif // GLMISC_HPP
