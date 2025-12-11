#ifndef GLSHADER_H
#define GLSHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

class GLShader
{
public:
    GLShader(const std::string& name);
    ~GLShader();

    void addVertexStage(const std::string& str);
    void addFragmentStage(const std::string& file);
    void addUniform(const std::string& uniform);
    void compile();
    GLuint getProgramID() const;
    GLint getUniformLocation(const std::string& uniform);
    std::string getName() const;

private:
    void checkShaderStatus(GLuint shaderID);
    void checkProgramStatus(GLuint programID);
    void add_shader(GLuint shaderID, const std::string& str);

    GLuint programID        = 0;
    GLuint vertexShaderID   = 0;
    GLuint geometryShaderID = 0;
    GLuint fragmentShaderID = 0;
    std::string name;
    std::map<std::string, GLint> uniforms;
};

#endif // GLSHADER_H
