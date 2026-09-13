//
//  cube.h
//  Grand Bedroom 3D Scene
//
//  A reusable unit cube (1x1x1, corner at the origin) with position +
//  normal attributes, drawn through Material uniforms so every piece of
//  furniture can be given its own color. Every room object is built by
//  scaling/translating/rotating this single cube (the 3D TRANSFORMATION
//  requirement), exactly like sphere.h wraps a reusable unit sphere.
//

#ifndef cube_h
#define cube_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

class Cube
{
public:
    Cube(glm::vec3 amb = glm::vec3(1.0f), glm::vec3 diff = glm::vec3(1.0f), glm::vec3 spec = glm::vec3(0.5f), float shiny = 32.0f)
        : ambient(amb), diffuse(diff), specular(spec), shininess(shiny)
    {
        float vertices[] = {
            // positions          // normals
            0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
            1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
            1.0f, 1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
            0.0f, 1.0f, 0.0f,   0.0f, 0.0f, -1.0f,

            1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 0.0f,

            0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,

            0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,

            1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,

            0.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
            1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
            1.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f
        };
        unsigned int indices[] = {
            0, 3, 2,  2, 1, 0,
            4, 5, 7,  7, 6, 4,
            8, 9, 10, 10, 11, 8,
            12, 13, 14, 14, 15, 12,
            16, 17, 18, 18, 19, 16,
            20, 21, 22, 22, 23, 20
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    ~Cube() {}

    // draw with the cube's own stored material color
    void drawCube(Shader& lightingShader, glm::mat4 model) const
    {
        drawCube(lightingShader, model, ambient, diffuse, specular, shininess);
    }

    // draw with an explicit RGB color (quick "different colors" helper,
    // mirrors the drawCube(...) free function from the reference project)
    void drawCube(Shader& lightingShader, glm::mat4 model, glm::vec3 colorAmbient, glm::vec3 colorDiffuse, glm::vec3 colorSpecular, float shiny) const
    {
        lightingShader.use();
        lightingShader.setVec3("material.ambient", colorAmbient);
        lightingShader.setVec3("material.diffuse", colorDiffuse);
        lightingShader.setVec3("material.specular", colorSpecular);
        lightingShader.setFloat("material.shininess", shiny);
        lightingShader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    // convenience overload: one RGB color used for ambient+diffuse, a
    // gentle default specular/shininess (this is the one most of the
    // furniture-building files call).
    void drawCubeRGB(Shader& lightingShader, glm::mat4 model, float r, float g, float b, float shiny = 32.0f, float specStrength = 0.3f) const
    {
        drawCube(lightingShader, model, glm::vec3(r, g, b) * 0.6f, glm::vec3(r, g, b), glm::vec3(specStrength), shiny);
    }

    unsigned int getVAO() const { return VAO; }

private:
    unsigned int VAO, VBO, EBO;
    glm::vec3 ambient, diffuse, specular;
    float shininess;
};

#endif /* cube_h */
