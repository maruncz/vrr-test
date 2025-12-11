#include "strip.hpp"
#include "glmisc.hpp"
#include <GL/glew.h>
#include <memory>
#include <string>

void Strip::draw(float pos)
{
    if (shader == nullptr)
    {
        initShader();
    }

    glUseProgram(shader->getProgramID());
    glUniform1f(posUniformLocation, pos);
    glBindVertexArray(VAOID);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    GLMisc::checkGLerror(HERE);
}

void Strip::initShader()
{
    shader = std::make_unique<GLShader>("strip");
    shader->addVertexStage(std::string(vertexShader));
    shader->addFragmentStage(std::string(fragmentShader));
    shader->compile();
    shader->addUniform("pos");

    posUniformLocation = shader->getUniformLocation("pos");

    glUseProgram(shader->getProgramID());

    glGenVertexArrays(1, &VAOID);
    glBindVertexArray(VAOID);

    glGenBuffers(1, &VBOID);
    glBindBuffer(GL_ARRAY_BUFFER, VBOID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 2UL * sizeof(GLfloat),
                 vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    GLMisc::checkGLerror(HERE);
}
