#ifndef GLMISC_HPP
#define GLMISC_HPP


#include <GL/glew.h>
#include <iomanip>
#include <iostream>
#include <sstream>

/**
 * @brief macro for easy backtracing
 * @details this macro combines `__FILE__`, `__func__` and `__LINE__` macros to
 * fill GLMisc::checkGLerror anf GLMisc::checkGLFBstatus arguments
 */
#define HERE __FILE__, __func__, __LINE__

namespace GLMisc
{

/**
 * @brief checks if opengl set error
 * @param file name of file where error occurred
 * @param func name of function where error occurred
 * @param line line number where error occurred
 * @throw std::runtime_error with position and error string
 * @note this function does nothing in Release mode
 */
#ifdef NDEBUG
inline void checkGLerror(const char *, const char *, unsigned)
{
#else
inline void checkGLerror(const char *file, const char *func, unsigned line)
{
    std::stringstream s;
    switch (glGetError())
    {
    case GL_INVALID_ENUM:
        s << file << ": " << func << ": " << line << ": "
          << "GL_INVALID_ENUM" << std::endl;
        throw std::runtime_error(s.str());
    case GL_INVALID_VALUE:
        s << file << ": " << func << ": " << line << ": "
          << "GL_INVALID_VALUE" << std::endl;
        throw std::runtime_error(s.str());
    case GL_INVALID_OPERATION:
        s << file << ": " << func << ": " << line << ": "
          << "GL_INVALID_OPERATION" << std::endl;
        throw std::runtime_error(s.str());
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        s << file << ": " << func << ": " << line << ": "
          << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
        throw std::runtime_error(s.str());
    case GL_OUT_OF_MEMORY:
        s << file << ": " << func << ": " << line << ": "
          << "GL_OUT_OF_MEMORY" << std::endl;
        throw std::runtime_error(s.str());
    case GL_STACK_UNDERFLOW:
        s << file << ": " << func << ": " << line << ": "
          << "GL_STACK_UNDERFLOW" << std::endl;
        throw std::runtime_error(s.str());
    case GL_STACK_OVERFLOW:
        s << file << ": " << func << ": " << line << ": "
          << "GL_STACK_OVERFLOW" << std::endl;
        throw std::runtime_error(s.str());
    default: break;
    }
#endif
}

}

#endif // GLMISC_HPP
