#ifndef STRIP_HPP
#define STRIP_HPP

#include "glshader.h"
#include <GL/glew.h>
#include <memory>
#include <string_view>

class Strip
{
public:
    void draw(float pos);

private:
    void initShader();

    GLuint VBOID;
    GLuint VAOID;
    GLint posUniformLocation;

    std::unique_ptr<GLShader> shader;

    static constexpr float width {0.1f};
    static constexpr glm::vec2 vertices[4] = {
        // clang-format off
        glm::vec2(-1.0f * width, -1.0f),
        glm::vec2( 1.0f * width, -1.0f),
        glm::vec2(-1.0f * width,  1.0f),
        glm::vec2( 1.0f * width,  1.0f)
        // clang-format on
    };

    static constexpr std::string_view vertexShader = R"(
#version 400 core

layout(location=0) in vec2 vertex;

uniform float pos;

void main(void)
{
    gl_Position = vec4(vertex.x + pos, vertex.y, 0.0f, 1.0f);
}
)";

    static constexpr std::string_view fragmentShader = R"(
#version 400 core

out vec4 fColor;

void main(void)
{
    fColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
)";
};

#endif // STRIP_HPP
