#include "glshader.h"
#include "glmisc.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>

GLShader::GLShader(std::string_view name)
    : programID(glCreateProgram()), name(name)
{
}

GLShader::~GLShader()
{
    glDeleteProgram(programID);
}

void GLShader::addVertexStage(const std::string& str)
{
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    add_shader(vertexShaderID, str);
}

void GLShader::addFragmentStage(const std::string& file)
{
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    add_shader(fragmentShaderID, file);
}

void GLShader::addUniform(const std::string& uniform)
{
    auto it = uniforms.find(uniform);
    if (it != uniforms.end())
    {
        return;
    }
    /// @todo co kdyz je odoptimalizovany
    GLint const loc = glGetUniformLocation(programID, uniform.c_str());
    if (loc == -1)
    {
        std::stringstream s;
        s << __FILE__ << ": " << __func__ << ": " << __LINE__ << ": "
          << "no such uniform: " << uniform << '\n';
        throw std::runtime_error(s.str());
    }
    uniforms.insert({uniform, loc});
}

void GLShader::compile()
{
    glLinkProgram(programID);
    try
    {
        checkProgramStatus(programID);
    }
    catch (const std::runtime_error err)
    {
        std::stringstream s;
        s << __FILE__ << ": " << __func__ << ": " << __LINE__ << ": "
          << "error linking shader program: " << '\n'
          << err.what();
        throw std::runtime_error(s.str());
    }

    if (glIsShader(vertexShaderID))
    {
        glDetachShader(programID, vertexShaderID);
        glDeleteShader(vertexShaderID);
    }
    if (glIsShader(geometryShaderID))
    {
        glDetachShader(programID, geometryShaderID);
        glDeleteShader(geometryShaderID);
    }
    if (glIsShader(fragmentShaderID))
    {
        glDetachShader(programID, fragmentShaderID);
        glDeleteShader(fragmentShaderID);
    }

    GLMisc::checkGLerror(HERE);
}

GLuint GLShader::getProgramID() const
{
    return programID;
}

GLint GLShader::getUniformLocation(const std::string& uniform)
{
    auto it = uniforms.find(uniform);
    if (it == uniforms.end())
    {
        std::stringstream s;
        s << __FILE__ << ": " << __func__ << ": " << __LINE__ << ": "
          << "no such uniform: " << uniform << '\n';
        throw std::runtime_error(s.str());
    }
    return it->second;
}

void GLShader::checkShaderStatus(GLuint shaderID)
{
    GLint compileStatus = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE)
    {
        GLint infoLogLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<GLchar> buffer(infoLogLength);

        glGetShaderInfoLog(shaderID, infoLogLength, nullptr, buffer.data());
        std::stringstream s;
        s << __FILE__ << ": " << __func__ << ": " << __LINE__ << ": "
          << buffer.data() << '\n';
        throw std::runtime_error(s.str());
    }
}

void GLShader::checkProgramStatus(GLuint programID)
{
    GLint linkStatus = 0;
    glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE)
    {
        GLint infoLogLength = 0;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<GLchar> buffer(infoLogLength);

        glGetProgramInfoLog(programID, infoLogLength, nullptr, buffer.data());
        std::stringstream s;
        s << __FILE__ << ": " << __func__ << ": " << __LINE__ << ": "
          << buffer.data() << '\n';
        throw std::runtime_error(s.str());
    }
}

void GLShader::add_shader(GLuint shaderID, const std::string& str)
{
    const auto* const ptr = str.c_str();
    glShaderSource(shaderID, 1, &ptr, nullptr);
    glCompileShader(shaderID);
    try
    {
        checkShaderStatus(shaderID);
    }
    catch (const std::runtime_error err)
    {
        std::stringstream s;
        s << __FILE__ << ": " << __func__ << ": " << __LINE__ << ": "
          << "error compiling shader: " << str << '\n'
          << err.what();
        throw std::runtime_error(s.str());
    }
    glAttachShader(programID, shaderID);
}
