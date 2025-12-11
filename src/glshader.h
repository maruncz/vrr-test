#ifndef GLSHADER_H
#define GLSHADER_H

#include <GL/glew.h>
#include <map>
#include <string>
#include <string_view>

class GLShader
{
public:
    GLShader(std::string_view name);
    ~GLShader();

    void addVertexStage(const std::string& str);
    void addFragmentStage(const std::string& file);
    void addUniform(const std::string& uniform);
    void compile();
    [[nodiscard]] GLuint getProgramID() const;
    GLint getUniformLocation(const std::string& uniform);
    [[nodiscard]] std::string getName() const { return name; }

private:
    static void checkShaderStatus(GLuint shaderID);
    static void checkProgramStatus(GLuint programID);
    void add_shader(GLuint shaderID, const std::string& str);

    GLuint programID        = 0;
    GLuint vertexShaderID   = 0;
    GLuint geometryShaderID = 0;
    GLuint fragmentShaderID = 0;
    std::string name;
    std::map<std::string, GLint> uniforms;
};

#endif // GLSHADER_H
