#include "glshader.h"
#include "glmisc.hpp"
#include <iostream>
#include <sstream>

GLShader::GLShader(const std::string& name)
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
    GLint loc = glGetUniformLocation(programID, uniform.c_str());
    if (loc == -1)
    {
        std::stringstream s;
        s << __FILE__ << ": " << __func__ << ": " << __LINE__ << ": "
          << "no such uniform: " << uniform << std::endl;
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
    catch (std::string err)
    {
        std::stringstream s;
        s << __FILE__ << ": " << __func__ << ": " << __LINE__ << ": "
          << "error linking shader program: " << std::endl
          << err;
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
          << "no such uniform: " << uniform << std::endl;
        throw std::runtime_error(s.str());
    }
    return it->second;
}

void GLShader::checkShaderStatus(GLuint shaderID)
{
    GLint compileStatus;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE)
    {
        GLint infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        auto* buffer = new GLchar[infoLogLength]();

        glGetShaderInfoLog(shaderID, infoLogLength, nullptr, buffer);
        std::stringstream s;
        s << __FILE__ << ": " << __func__ << ": " << __LINE__ << ": " << buffer
          << std::endl;
        delete[] buffer;
        throw std::runtime_error(s.str());
    }
}

void GLShader::checkProgramStatus(GLuint programID)
{
    GLint linkStatus;
    glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE)
    {
        GLint infoLogLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        auto* buffer = new GLchar[infoLogLength]();

        glGetProgramInfoLog(programID, infoLogLength, nullptr, buffer);
        std::stringstream s;
        s << __FILE__ << ": " << __func__ << ": " << __LINE__ << ": " << buffer
          << std::endl;
        delete[] buffer;
        throw std::runtime_error(s.str());
    }
}

void GLShader::add_shader(GLuint shaderID, const std::string& str)
{
    const auto ptr = str.c_str();
    glShaderSource(shaderID, 1, &ptr, nullptr);
    glCompileShader(shaderID);
    try
    {
        checkShaderStatus(shaderID);
    }
    catch (std::string err)
    {
        std::stringstream s;
        s << __FILE__ << ": " << __func__ << ": " << __LINE__ << ": "
          << "error compiling shader: " << str << std::endl
          << err;
        throw std::runtime_error(s.str());
    }
    glAttachShader(programID, shaderID);
}

std::string GLShader::getName() const
{
    return name;
}
