//
//  decor.h
//  Grand Bedroom 3D Scene
//
//  TASK: decorative light fixtures + a picture frame. This file also
//  defines the WORLD-SPACE positions used to place the actual PointLight /
//  SpotLight objects in main.cpp, so every visible lamp/fixture lines up
//  with a real light source lighting the room.
//

#ifndef decor_h
#define decor_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "cube.h"
#include "sphere.h"
#include "room.h"
#include "bed.h"
#include "furniture.h"

// ---- Light source anchor points (used by main.cpp to build PointLight /
//      SpotLight objects, and here to draw their visible fixtures) --------
const glm::vec3 CHANDELIER_POS(ROOM_W / 2.0f, ROOM_H - 0.18f, ROOM_D / 2.0f);

const float NIGHTSTAND_X = BED_X1 - 0.50f;
const float NIGHTSTAND_TOP_Y = 0.55f;
const glm::vec3 NIGHTSTAND1_POS(NIGHTSTAND_X + 0.20f, NIGHTSTAND_TOP_Y, BED_Z0 - 0.45f);
const glm::vec3 NIGHTSTAND2_POS(NIGHTSTAND_X + 0.20f, NIGHTSTAND_TOP_Y, BED_Z1 + 0.45f);
const glm::vec3 BEDSIDE_LAMP1_POS = NIGHTSTAND1_POS + glm::vec3(0.0f, 0.45f, 0.0f);
const glm::vec3 BEDSIDE_LAMP2_POS = NIGHTSTAND2_POS + glm::vec3(0.0f, 0.45f, 0.0f);

const glm::vec3 FLOOR_LAMP_BASE(0.80f, 0.0f, 1.55f);
const glm::vec3 FLOOR_LAMP_LIGHT_POS = FLOOR_LAMP_BASE + glm::vec3(0.0f, 1.45f, 0.0f);

const glm::vec3 DESK_LAMP_POS(DESK_X0 + 1.10f, DESK_TOP_Y + 0.38f, DESK_Z0 + 0.45f);
const glm::vec3 DESK_LAMP_DIR = glm::normalize(glm::vec3(-0.15f, -1.0f, 0.35f));

inline void decorBox(const Cube& cube, Shader& shader, const glm::mat4& sceneModel,
                      float x, float y, float z, float sx, float sy, float sz,
                      float r, float g, float b, float shininess = 32.0f, float spec = 0.3f)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    model = glm::scale(model, glm::vec3(sx, sy, sz));
    cube.drawCubeRGB(shader, sceneModel * model, r, g, b, shininess, spec);
}

inline void decorSphere(const Sphere& sphere, Shader& shader, const glm::mat4& sceneModel,
                         glm::vec3 pos, float radius, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shininess)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
    model = glm::scale(model, glm::vec3(radius));
    sphere.drawSphere(shader, sceneModel * model, amb, diff, spec, shininess);
}

// Chandelier hanging from the ceiling — brass ring + a warm glass globe
// that visually marks pointLight #1.
inline void buildChandelier(const Cube& cube, const Sphere& sphere, Shader& shader, const glm::mat4& sceneModel)
{
    decorBox(cube, shader, sceneModel, CHANDELIER_POS.x - 0.015f, CHANDELIER_POS.y, CHANDELIER_POS.z - 0.015f, 0.03f, 0.16f, 0.03f, 0.75f, 0.60f, 0.20f, 90.0f, 0.7f);
    decorSphere(sphere, shader, sceneModel, CHANDELIER_POS, 0.16f, glm::vec3(0.35f, 0.30f, 0.10f), glm::vec3(0.95f, 0.85f, 0.55f), glm::vec3(0.8f), 96.0f);
}

// Two bedside nightstands + lamps — visually mark pointLights #2 and #3.
inline void buildNightstand(const Cube& cube, const Sphere& sphere, Shader& shader, const glm::mat4& sceneModel, glm::vec3 tablePos, glm::vec3 lampPos)
{
    decorBox(cube, shader, sceneModel, tablePos.x - 0.24f, 0.0f, tablePos.z - 0.24f, 0.48f, tablePos.y, 0.48f, 0.32f, 0.18f, 0.10f, 26.0f, 0.2f);
    decorBox(cube, shader, sceneModel, lampPos.x - 0.02f, tablePos.y, lampPos.z - 0.02f, 0.04f, 0.30f, 0.04f, 0.70f, 0.55f, 0.20f, 80.0f, 0.6f);
    decorSphere(sphere, shader, sceneModel, lampPos, 0.14f, glm::vec3(0.35f, 0.25f, 0.08f), glm::vec3(0.95f, 0.75f, 0.35f), glm::vec3(0.6f), 60.0f);
}

// A tall corner floor lamp — visually marks pointLight #4.
inline void buildFloorLamp(const Cube& cube, const Sphere& sphere, Shader& shader, const glm::mat4& sceneModel)
{
    decorBox(cube, shader, sceneModel, FLOOR_LAMP_BASE.x - 0.12f, 0.0f, FLOOR_LAMP_BASE.z - 0.12f, 0.24f, 0.04f, 0.24f, 0.15f, 0.15f, 0.16f, 30.0f, 0.3f);
    decorBox(cube, shader, sceneModel, FLOOR_LAMP_BASE.x - 0.015f, 0.04f, FLOOR_LAMP_BASE.z - 0.015f, 0.03f, 1.35f, 0.03f, 0.20f, 0.20f, 0.22f, 40.0f, 0.3f);
    decorSphere(sphere, shader, sceneModel, FLOOR_LAMP_LIGHT_POS, 0.20f, glm::vec3(0.30f, 0.25f, 0.12f), glm::vec3(0.85f, 0.72f, 0.40f), glm::vec3(0.5f), 40.0f);
}

// Desk reading lamp — visually marks the SpotLight, aimed down at the desk.
inline void buildDeskLampFixture(const Cube& cube, const Sphere& sphere, Shader& shader, const glm::mat4& sceneModel)
{
    // base
    decorBox(cube, shader, sceneModel, DESK_LAMP_POS.x - 0.06f, DESK_TOP_Y + 0.02f, DESK_LAMP_POS.z - 0.06f, 0.12f, 0.03f, 0.12f, 0.10f, 0.10f, 0.11f, 40.0f, 0.4f);
    // arm
    decorBox(cube, shader, sceneModel, DESK_LAMP_POS.x - 0.015f, DESK_TOP_Y + 0.05f, DESK_LAMP_POS.z - 0.015f, 0.03f, 0.32f, 0.03f, 0.12f, 0.12f, 0.13f, 40.0f, 0.4f);
    // shade (small cone-ish box tilted toward the desk), bright so the
    // spotlight source itself reads clearly against the room
    glm::mat4 shade = glm::translate(glm::mat4(1.0f), DESK_LAMP_POS);
    shade = glm::rotate(shade, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shade = glm::translate(shade, glm::vec3(-0.08f, -0.05f, -0.08f));
    shade = glm::scale(shade, glm::vec3(0.16f, 0.10f, 0.16f));
    cube.drawCubeRGB(shader, sceneModel * shade, 0.95f, 0.55f, 0.15f, 60.0f, 0.5f);
}

// A framed piece of art on the left wall, purely for extra color variety.
inline void buildPictureFrame(const Cube& cube, Shader& shader, const glm::mat4& sceneModel)
{
    decorBox(cube, shader, sceneModel, WALL_T, 1.8f, 3.4f, 0.03f, 1.1f, 1.5f, 0.55f, 0.42f, 0.15f, 60.0f, 0.5f);
    decorBox(cube, shader, sceneModel, WALL_T + 0.031f, 1.88f, 3.48f, 0.01f, 0.94f, 1.34f, 0.25f, 0.45f, 0.60f, 15.0f, 0.1f);
    decorBox(cube, shader, sceneModel, WALL_T + 0.033f, 2.25f, 3.9f, 0.005f, 0.20f, 0.55f, 0.90f, 0.70f, 0.15f, 15.0f, 0.1f);
}

#endif /* decor_h */
