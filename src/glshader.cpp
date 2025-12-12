#include "glshader.h"
#include "glmisc.hpp"
#include "misc.hpp"
#include <format>
#include <source_location>
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
    GLint const loc = glGetUniformLocation(programID, uniform.c_str());
    if (loc == -1)
    {
        throw std::runtime_error(std::format("{}: no such uniform: {}\n",
                                             std::source_location::current(),
                                             uniform));
    }
    uniforms.insert({uniform, loc});
}

void GLShader::compile() const
{
    glLinkProgram(programID);
    try
    {
        checkProgramStatus(programID);
    }
    catch (const std::runtime_error& err)
    {
        throw std::runtime_error(
            std::format("{}: error linking shader program:\n{}\n",
                        std::source_location::current(), err.what()));
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

    GLMisc::checkGLerror();
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
        throw std::runtime_error(std::format("{}: no such uniform: {}\n",
                                             std::source_location::current(),
                                             uniform));
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
        throw std::runtime_error(std::format(
            "{}: {}\n", std::source_location::current(), buffer.data()));
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
        throw std::runtime_error(std::format(
            "{}: {}\n", std::source_location::current(), buffer.data()));
    }
}

void GLShader::add_shader(GLuint shaderID, const std::string& str) const
{
    const auto* const ptr = str.c_str();
    glShaderSource(shaderID, 1, &ptr, nullptr);
    glCompileShader(shaderID);
    try
    {
        checkShaderStatus(shaderID);
    }
    catch (const std::runtime_error& err)
    {
        throw std::runtime_error(
            std::format("{}: error compiling shader {}:\n{}\n",
                        std::source_location::current(), str, err.what()));
    }
    glAttachShader(programID, shaderID);
}
