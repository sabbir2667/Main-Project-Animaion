//
//  room.h
//  Grand Bedroom 3D Scene
//
//  TASK: Room shell — floor, ceiling, four walls (with a window gap in the
//  back wall), a window with glass + frame, and curtains.
//  Every piece is the shared unit Cube run through its own
//  translate * scale 3D TRANSFORMATION, then combined with the scene's
//  outer "sceneModel" matrix (so pressing the scene rotate/scale/translate
//  keys in main.cpp moves the whole room together).
//
//  Room footprint: X:[0,ROOM_W]  Y:[0,ROOM_H]  Z:[0,ROOM_D]
//

#ifndef room_h
#define room_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "cube.h"

const float ROOM_W = 8.0f;
const float ROOM_H = 4.5f;
const float ROOM_D = 8.0f;
const float WALL_T = 0.08f;

// draws a unit-cube-based box at (x,y,z) sized (sx,sy,sz) with color (r,g,b)
inline void roomBox(const Cube& cube, Shader& shader, const glm::mat4& sceneModel,
                     float x, float y, float z, float sx, float sy, float sz,
                     float r, float g, float b, float shininess = 16.0f, float spec = 0.15f)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    model = glm::scale(model, glm::vec3(sx, sy, sz));
    cube.drawCubeRGB(shader, sceneModel * model, r, g, b, shininess, spec);
}

inline void buildRoom(const Cube& cube, Shader& shader, const glm::mat4& sceneModel)
{
    // ---- Floor: warm oak brown ----
    roomBox(cube, shader, sceneModel, 0.0f, 0.0f, 0.0f, ROOM_W, WALL_T, ROOM_D, 0.42f, 0.27f, 0.15f, 24.0f, 0.25f);

    // ---- Ceiling: soft off-white plaster ----
    roomBox(cube, shader, sceneModel, 0.0f, ROOM_H - WALL_T, 0.0f, ROOM_W, WALL_T, ROOM_D, 0.93f, 0.92f, 0.88f, 8.0f, 0.05f);

    // ---- Left wall (x = 0): dusty rose ----
    roomBox(cube, shader, sceneModel, 0.0f, 0.0f, 0.0f, WALL_T, ROOM_H, ROOM_D, 0.72f, 0.55f, 0.55f);

    // ---- Right wall (x = ROOM_W): dusty rose ----
    roomBox(cube, shader, sceneModel, ROOM_W - WALL_T, 0.0f, 0.0f, WALL_T, ROOM_H, ROOM_D, 0.72f, 0.55f, 0.55f);

    // ---- Back wall (z = 0) split around a window opening ----
    float winX0 = 1.6f, winX1 = 6.4f;   // window horizontal span
    float winY0 = 1.3f, winY1 = 3.1f;   // window vertical span
    glm::vec3 wallColor(0.80f, 0.74f, 0.66f);
    // bottom band
    roomBox(cube, shader, sceneModel, 0.0f, 0.0f, 0.0f, ROOM_W, winY0, WALL_T, wallColor.r, wallColor.g, wallColor.b);
    // top band
    roomBox(cube, shader, sceneModel, 0.0f, winY1, 0.0f, ROOM_W, ROOM_H - winY1, WALL_T, wallColor.r, wallColor.g, wallColor.b);
    // left pier
    roomBox(cube, shader, sceneModel, 0.0f, winY0, 0.0f, winX0, winY1 - winY0, WALL_T, wallColor.r, wallColor.g, wallColor.b);
    // right pier
    roomBox(cube, shader, sceneModel, winX1, winY0, 0.0f, ROOM_W - winX1, winY1 - winY0, WALL_T, wallColor.r, wallColor.g, wallColor.b);

    // ---- Window glass: cool blue-grey ----
    roomBox(cube, shader, sceneModel, winX0 + 0.06f, winY0 + 0.06f, 0.02f, (winX1 - winX0) - 0.12f, (winY1 - winY0) - 0.12f, 0.03f, 0.55f, 0.68f, 0.78f, 96.0f, 0.6f);

    // ---- Window frame: dark walnut ----
    float fr = 0.07f;
    roomBox(cube, shader, sceneModel, winX0, winY0, -0.01f, winX1 - winX0, fr, WALL_T + 0.02f, 0.25f, 0.16f, 0.08f);
    roomBox(cube, shader, sceneModel, winX0, winY1 - fr, -0.01f, winX1 - winX0, fr, WALL_T + 0.02f, 0.25f, 0.16f, 0.08f);
    roomBox(cube, shader, sceneModel, winX0, winY0, -0.01f, fr, winY1 - winY0, WALL_T + 0.02f, 0.25f, 0.16f, 0.08f);
    roomBox(cube, shader, sceneModel, winX1 - fr, winY0, -0.01f, fr, winY1 - winY0, WALL_T + 0.02f, 0.25f, 0.16f, 0.08f);
    // mullion (centre divider)
    roomBox(cube, shader, sceneModel, (winX0 + winX1) / 2.0f - fr / 2.0f, winY0, -0.01f, fr, winY1 - winY0, WALL_T + 0.02f, 0.25f, 0.16f, 0.08f);

    // ---- Curtains flanking the window: deep burgundy ----
    roomBox(cube, shader, sceneModel, winX0 - 0.55f, 0.55f, 0.05f, 0.35f, ROOM_H - 1.05f, 0.10f, 0.42f, 0.10f, 0.14f, 6.0f, 0.05f);
    roomBox(cube, shader, sceneModel, winX1 + 0.20f, 0.55f, 0.05f, 0.35f, ROOM_H - 1.05f, 0.10f, 0.42f, 0.10f, 0.14f, 6.0f, 0.05f);
    // curtain rod: brushed gold
    roomBox(cube, shader, sceneModel, winX0 - 0.6f, ROOM_H - 1.0f, 0.08f, (winX1 - winX0) + 1.0f, 0.06f, 0.06f, 0.80f, 0.65f, 0.22f, 90.0f, 0.7f);

    // ---- Baseboard trim: white ----
    roomBox(cube, shader, sceneModel, 0.0f, WALL_T, 0.0f, ROOM_W, 0.10f, WALL_T + 0.02f, 0.9f, 0.9f, 0.88f);
    roomBox(cube, shader, sceneModel, 0.0f, WALL_T, 0.0f, WALL_T + 0.02f, 0.10f, ROOM_D, 0.9f, 0.9f, 0.88f);
    roomBox(cube, shader, sceneModel, ROOM_W - WALL_T - 0.02f, WALL_T, 0.0f, WALL_T + 0.02f, 0.10f, ROOM_D, 0.9f, 0.9f, 0.88f);

    // ---- Area rug in front of the bed: teal ----
    roomBox(cube, shader, sceneModel, 1.0f, WALL_T + 0.001f, 4.4f, 3.6f, 0.02f, 2.6f, 0.16f, 0.45f, 0.42f, 4.0f, 0.02f);
}

#endif /* room_h */
