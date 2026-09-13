//
//  furniture.h
//  Grand Bedroom 3D Scene
//
//  TASK: extra furniture — a wardrobe with a SLIDING DOOR (a second moving
//  object driven by main.cpp's doorOffset, animated with glm::translate),
//  a study desk + chair (this is where the reading SpotLight sits), and a
//  bookshelf. Everything is built out of the shared unit Cube.
//

#ifndef furniture_h
#define furniture_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "cube.h"

inline void furnBox(const Cube& cube, Shader& shader, const glm::mat4& sceneModel,
    float x, float y, float z, float sx, float sy, float sz,
    float r, float g, float b, float shininess = 24.0f, float spec = 0.2f)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    model = glm::scale(model, glm::vec3(sx, sy, sz));
    cube.drawCubeRGB(shader, sceneModel * model, r, g, b, shininess, spec);
}

// ---------------------------------------------------------------------
// Wardrobe: fixed carcass + two doors that slide apart along Z.
// doorOffset: 0.0 = fully closed, 1.0 = fully open.
// ---------------------------------------------------------------------
const float WARDROBE_X0 = 0.10f;
const float WARDROBE_X1 = 0.65f;
const float WARDROBE_Z0 = 4.60f;
const float WARDROBE_Z1 = 6.60f;
const float WARDROBE_H = 2.30f;

inline void buildWardrobe(const Cube& cube, Shader& shader, const glm::mat4& sceneModel, float doorOffset)
{
    float wx = WARDROBE_X1 - WARDROBE_X0;
    float wz = WARDROBE_Z1 - WARDROBE_Z0;

    // Carcass (body) — rich mahogany
    furnBox(cube, shader, sceneModel, WARDROBE_X0, 0.0f, WARDROBE_Z0, wx, WARDROBE_H, wz, 0.30f, 0.14f, 0.09f, 26.0f, 0.2f);
    // Top cornice — slightly darker
    furnBox(cube, shader, sceneModel, WARDROBE_X0 - 0.03f, WARDROBE_H, WARDROBE_Z0 - 0.03f, wx + 0.06f, 0.08f, wz + 0.06f, 0.22f, 0.10f, 0.06f, 26.0f, 0.2f);

    // Sliding doors — each half the wardrobe's width, honey-oak veneer,
    // slid apart by doorOffset (up to 45% of the panel width each way).
    float doorW = wz / 2.0f - 0.02f;
    float slide = doorOffset * (doorW * 0.9f);
    float doorDepth = 0.035f;
    float doorFront = WARDROBE_Z0 - doorDepth - 0.005f; // just in front of the carcass face... but face is +X, adjust below
    // NOTE: the wardrobe's open face looks along +X (out of the carcass),
    // so the doors sit at x = WARDROBE_X1 (front face) and slide in Z.
    float faceX = WARDROBE_X1 + 0.005f;

    // left-hand door slides toward -Z when opening
    furnBox(cube, shader, sceneModel, faceX, 0.05f, WARDROBE_Z0 + 0.01f - slide, doorDepth, WARDROBE_H - 0.10f, doorW, 0.62f, 0.42f, 0.20f, 40.0f, 0.35f);
    // right-hand door slides toward +Z when opening
    furnBox(cube, shader, sceneModel, faceX, 0.05f, WARDROBE_Z0 + doorW + 0.03f + slide, doorDepth, WARDROBE_H - 0.10f, doorW, 0.62f, 0.42f, 0.20f, 40.0f, 0.35f);

    // Door handles — brass
    furnBox(cube, shader, sceneModel, faceX + doorDepth, 1.05f, WARDROBE_Z0 + doorW - 0.06f - slide, 0.02f, 0.14f, 0.02f, 0.80f, 0.65f, 0.22f, 90.0f, 0.7f);
    furnBox(cube, shader, sceneModel, faceX + doorDepth, 1.05f, WARDROBE_Z0 + doorW + 0.09f + slide, 0.02f, 0.14f, 0.02f, 0.80f, 0.65f, 0.22f, 90.0f, 0.7f);

    (void)doorFront;
}

// ---------------------------------------------------------------------
// Study desk + chair — sits in the back-left corner, under the reading
// SpotLight (see decor.h / DESK_LAMP_POS).
// ---------------------------------------------------------------------
const float DESK_X0 = 0.20f;
const float DESK_Z0 = 0.20f;
const float DESK_W = 1.55f;
const float DESK_D = 0.70f;
const float DESK_TOP_Y = 0.72f;

inline void buildStudyDesk(const Cube& cube, Shader& shader, const glm::mat4& sceneModel)
{
    // Desktop — light ash wood
    furnBox(cube, shader, sceneModel, DESK_X0, DESK_TOP_Y, DESK_Z0, DESK_W, 0.04f, DESK_D, 0.80f, 0.70f, 0.55f, 30.0f, 0.25f);

    // Four legs — matte black
    float legT = 0.05f;
    furnBox(cube, shader, sceneModel, DESK_X0 + 0.03f, 0.0f, DESK_Z0 + 0.03f, legT, DESK_TOP_Y, legT, 0.08f, 0.08f, 0.09f, 20.0f, 0.15f);
    furnBox(cube, shader, sceneModel, DESK_X0 + DESK_W - 0.03f - legT, 0.0f, DESK_Z0 + 0.03f, legT, DESK_TOP_Y, legT, 0.08f, 0.08f, 0.09f, 20.0f, 0.15f);
    furnBox(cube, shader, sceneModel, DESK_X0 + 0.03f, 0.0f, DESK_Z0 + DESK_D - 0.03f - legT, legT, DESK_TOP_Y, legT, 0.08f, 0.08f, 0.09f, 20.0f, 0.15f);
    furnBox(cube, shader, sceneModel, DESK_X0 + DESK_W - 0.03f - legT, 0.0f, DESK_Z0 + DESK_D - 0.03f - legT, legT, DESK_TOP_Y, legT, 0.08f, 0.08f, 0.09f, 20.0f, 0.15f);

    // A book on the desk — small teal box, for a splash of color
    furnBox(cube, shader, sceneModel, DESK_X0 + 0.90f, DESK_TOP_Y + 0.04f, DESK_Z0 + 0.15f, 0.28f, 0.05f, 0.20f, 0.10f, 0.45f, 0.42f, 12.0f, 0.1f);

    // Chair — seat + backrest + legs, deep green
    float chairX = DESK_X0 + 0.55f, chairZ = DESK_Z0 + DESK_D + 0.35f;
    furnBox(cube, shader, sceneModel, chairX, 0.42f, chairZ, 0.45f, 0.04f, 0.45f, 0.10f, 0.35f, 0.22f, 20.0f, 0.15f);
    furnBox(cube, shader, sceneModel, chairX, 0.42f, chairZ + 0.40f, 0.45f, 0.55f, 0.04f, 0.10f, 0.35f, 0.22f, 20.0f, 0.15f);
    for (int i = 0; i < 4; i++)
    {
        float lx = (i % 2 == 0) ? chairX + 0.02f : chairX + 0.41f;
        float lz = (i / 2 == 0) ? chairZ + 0.02f : chairZ + 0.41f;
        furnBox(cube, shader, sceneModel, lx, 0.0f, lz, 0.03f, 0.42f, 0.03f, 0.05f, 0.05f, 0.05f, 15.0f, 0.1f);
    }
}

// ---------------------------------------------------------------------
// Bookshelf — tall unit on the right wall, near the front of the room.
// ---------------------------------------------------------------------
const float BOOKSHELF_X0 = 7.30f;
const float BOOKSHELF_Z0 = 6.00f;

inline void buildBookshelf(const Cube& cube, Shader& shader, const glm::mat4& sceneModel)
{
    float x0 = BOOKSHELF_X0, z0 = BOOKSHELF_Z0;
    // frame — dark walnut
    furnBox(cube, shader, sceneModel, x0, 0.0f, z0, 0.32f, 2.4f, 1.4f, 0.24f, 0.14f, 0.08f, 24.0f, 0.2f);
    // three shelves — cream
    for (int i = 0; i < 3; i++)
        furnBox(cube, shader, sceneModel, x0 + 0.02f, 0.60f + i * 0.55f, z0 + 0.02f, 0.28f, 0.03f, 1.36f, 0.85f, 0.83f, 0.75f, 16.0f, 0.1f);

    // a handful of colorful "books" for variety, standing upright
    const float bookColors[6][3] = {
        {0.75f, 0.20f, 0.20f}, {0.20f, 0.45f, 0.75f}, {0.85f, 0.65f, 0.15f},
        {0.30f, 0.60f, 0.35f}, {0.55f, 0.25f, 0.65f}, {0.90f, 0.45f, 0.15f}
    };
    for (int i = 0; i < 6; i++)
    {
        float bz = z0 + 0.10f + i * 0.20f;
        furnBox(cube, shader, sceneModel, x0 + 0.06f, 0.63f, bz, 0.20f, 0.28f, 0.15f,
            bookColors[i][0], bookColors[i][1], bookColors[i][2], 10.0f, 0.05f);
    }
}

#endif /* furniture_h */