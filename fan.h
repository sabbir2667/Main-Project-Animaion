//
//  fan.h
//  Grand Bedroom 3D Scene
//
//  TASK: the ONE MOVING OBJECT required by the assignment — a ceiling fan
//  whose blades keep rotating every frame. update(deltaTime) advances the
//  angle, draw(...) rebuilds the rotation 3D TRANSFORMATION (translate to
//  the hub, rotate by the current angle, then place each blade) every
//  single frame, so the fan spins continuously while the rest of the
//  scene stays put. Press 'F' in main.cpp to start/stop it.
//

#ifndef fan_h
#define fan_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "cube.h"
#include "sphere.h"

class CeilingFan
{
public:
    CeilingFan(glm::vec3 mountPos, float bladeLength = 1.05f, float speedDegPerSec = 260.0f)
        : position(mountPos), length(bladeLength), speed(speedDegPerSec), angle(0.0f), spinning(true) {}

    void update(float deltaTime)
    {
        if (spinning)
            angle += speed * deltaTime;
        if (angle > 360.0f)
            angle -= 360.0f;
    }

    void toggle() { spinning = !spinning; }
    bool isSpinning() const { return spinning; }

    void draw(const Cube& cube, const Sphere& sphere, Shader& shader, const glm::mat4& sceneModel) const
    {
        // Mounting rod from the ceiling down to the hub — brushed steel
        glm::mat4 rod = glm::translate(glm::mat4(1.0f), glm::vec3(position.x - 0.02f, position.y, position.z - 0.02f));
        rod = glm::scale(rod, glm::vec3(0.04f, 0.18f, 0.04f));
        cube.drawCubeRGB(shader, sceneModel * rod, 0.55f, 0.55f, 0.58f, 70.0f, 0.5f);

        // Central hub — dark bronze sphere
        glm::mat4 hub = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y - 0.18f, position.z));
        hub = glm::scale(hub, glm::vec3(0.14f));
        sphere.drawSphere(shader, sceneModel * hub, glm::vec3(0.18f, 0.14f, 0.05f), glm::vec3(0.35f, 0.28f, 0.12f), glm::vec3(0.6f), 60.0f);

        // Four rotating blades — warm cherry wood, evenly spaced, spun by 'angle'
        glm::vec3 bladeColor(0.55f, 0.24f, 0.12f);
        for (int i = 0; i < 4; i++)
        {
            glm::mat4 blade = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y - 0.18f, position.z));
            blade = glm::rotate(blade, glm::radians(angle + i * 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            blade = glm::translate(blade, glm::vec3(0.10f, -0.015f, -0.09f));
            blade = glm::scale(blade, glm::vec3(length, 0.03f, 0.18f));
            cube.drawCubeRGB(shader, sceneModel * blade, bladeColor.r, bladeColor.g, bladeColor.b, 20.0f, 0.15f);
        }
    }

private:
    glm::vec3 position;  // hub mounting point, just below the ceiling
    float length;
    float speed;
    float angle;
    bool spinning;
};

#endif /* fan_h */
