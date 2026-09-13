//
//  spotLight.h
//  Grand Bedroom 3D Scene
//
//  LIGHT TYPE #2: spotlight (a cone of light with a position AND a
//  direction, fading out between an inner and outer cutoff angle) — used
//  for the study-desk reading lamp. Mirrors the style of pointLight.h.
//

#ifndef spotLight_h
#define spotLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include "shader.h"

class SpotLight {
public:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float k_c;
    float k_l;
    float k_q;
    float cutOff;       // inner cone angle (degrees)
    float outerCutOff;  // outer cone angle (degrees), gives a soft edge
    int lightNumber;

    SpotLight(glm::vec3 pos, glm::vec3 dir,
              float ambR, float ambG, float ambB,
              float diffR, float diffG, float diffB,
              float specR, float specG, float specB,
              float constant, float linear, float quadratic,
              float innerCutOffDeg, float outerCutOffDeg,
              int num)
    {
        position = pos;
        direction = dir;
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
        k_c = constant;
        k_l = linear;
        k_q = quadratic;
        cutOff = innerCutOffDeg;
        outerCutOff = outerCutOffDeg;
        lightNumber = num;
    }

    void setUpSpotLight(Shader& lightingShader)
    {
        lightingShader.use();
        std::string base = "spotLights[" + std::to_string(lightNumber - 1) + "]";
        lightingShader.setVec3(base + ".position", position);
        lightingShader.setVec3(base + ".direction", direction);
        lightingShader.setVec3(base + ".ambient", ambientOn * ambient);
        lightingShader.setVec3(base + ".diffuse", diffuseOn * diffuse);
        lightingShader.setVec3(base + ".specular", specularOn * specular);
        lightingShader.setFloat(base + ".k_c", k_c);
        lightingShader.setFloat(base + ".k_l", k_l);
        lightingShader.setFloat(base + ".k_q", k_q);
        lightingShader.setFloat(base + ".cutOff", glm::cos(glm::radians(cutOff)));
        lightingShader.setFloat(base + ".outerCutOff", glm::cos(glm::radians(outerCutOff)));
    }

    void setPosition(glm::vec3 pos) { position = pos; }
    void setDirection(glm::vec3 dir) { direction = dir; }

    void turnOff() { ambientOn = 0.0; diffuseOn = 0.0; specularOn = 0.0; }
    void turnOn()  { ambientOn = 1.0; diffuseOn = 1.0; specularOn = 1.0; }
    bool isOn() const { return diffuseOn > 0.5f; }

private:
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};

#endif /* spotLight_h */
