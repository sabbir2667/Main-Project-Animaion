//
//  bed.h
//  Grand Bedroom 3D Scene
//
//  TASK: the double bed — headboard, frame, legs, mattress, blanket and
//  two pillows. Every part is a translate * scale 3D TRANSFORMATION of the
//  shared unit Cube (pillows use the shared unit Sphere, squashed flat).
//

#ifndef bed_h
#define bed_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "cube.h"
#include "sphere.h"

// Bed footprint (world coordinates, matches room.h's room box)
const float BED_X0 = 4.35f;
const float BED_X1 = 7.75f;   // headboard side, close to the right wall
const float BED_Z0 = 1.00f;
const float BED_Z1 = 4.30f;

inline void bedBox(const Cube& cube, Shader& shader, const glm::mat4& sceneModel,
                    float x, float y, float z, float sx, float sy, float sz,
                    float r, float g, float b, float shininess = 24.0f, float spec = 0.25f)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    model = glm::scale(model, glm::vec3(sx, sy, sz));
    cube.drawCubeRGB(shader, sceneModel * model, r, g, b, shininess, spec);
}

inline void buildBed(const Cube& cube, const Sphere& sphere, Shader& shader, const glm::mat4& sceneModel)
{
    float bw = BED_X1 - BED_X0;
    float bl = BED_Z1 - BED_Z0;

    // Headboard — deep navy tufted velvet
    bedBox(cube, shader, sceneModel, BED_X1 - 0.10f, 0.0f, BED_Z0 - 0.15f, 0.14f, 1.55f, bl + 0.30f, 0.10f, 0.13f, 0.30f, 20.0f, 0.2f);

    // Bed frame / base — dark walnut
    bedBox(cube, shader, sceneModel, BED_X0, 0.0f, BED_Z0, bw, 0.42f, bl, 0.24f, 0.14f, 0.08f, 30.0f, 0.2f);

    // Mattress — crisp white
    bedBox(cube, shader, sceneModel, BED_X0 + 0.06f, 0.42f, BED_Z0 + 0.06f, bw - 0.12f, 0.34f, bl - 0.12f, 0.95f, 0.95f, 0.95f, 12.0f, 0.1f);

    // Blanket / throw — mustard gold, folded over the foot of the bed
    bedBox(cube, shader, sceneModel, BED_X0 + 0.02f, 0.76f, BED_Z0 + 0.02f, bw - 0.04f, 0.10f, bl * 0.55f, 0.80f, 0.62f, 0.16f, 18.0f, 0.15f);

    // Pillows — two, warm coral, near the headboard
    glm::mat4 pillow1 = glm::translate(glm::mat4(1.0f), glm::vec3(BED_X1 - 0.55f, 0.62f, BED_Z0 + 0.55f));
    pillow1 = glm::scale(pillow1, glm::vec3(0.30f, 0.16f, 0.55f));
    sphere.drawSphere(shader, sceneModel * pillow1, glm::vec3(0.55f, 0.20f, 0.18f), glm::vec3(0.90f, 0.42f, 0.38f), glm::vec3(0.15f), 10.0f);

    glm::mat4 pillow2 = glm::translate(glm::mat4(1.0f), glm::vec3(BED_X1 - 0.55f, 0.62f, BED_Z1 - 0.55f));
    pillow2 = glm::scale(pillow2, glm::vec3(0.30f, 0.16f, 0.55f));
    sphere.drawSphere(shader, sceneModel * pillow2, glm::vec3(0.55f, 0.20f, 0.18f), glm::vec3(0.90f, 0.42f, 0.38f), glm::vec3(0.15f), 10.0f);

    // Four bed legs — brushed brass
    float legSize = 0.08f, legH = 0.10f;
    bedBox(cube, shader, sceneModel, BED_X0 + 0.05f, -legH, BED_Z0 + 0.05f, legSize, legH, legSize, 0.70f, 0.55f, 0.20f, 80.0f, 0.6f);
    bedBox(cube, shader, sceneModel, BED_X1 - 0.05f - legSize, -legH, BED_Z0 + 0.05f, legSize, legH, legSize, 0.70f, 0.55f, 0.20f, 80.0f, 0.6f);
    bedBox(cube, shader, sceneModel, BED_X0 + 0.05f, -legH, BED_Z1 - 0.05f - legSize, legSize, legH, legSize, 0.70f, 0.55f, 0.20f, 80.0f, 0.6f);
    bedBox(cube, shader, sceneModel, BED_X1 - 0.05f - legSize, -legH, BED_Z1 - 0.05f - legSize, legSize, legH, legSize, 0.70f, 0.55f, 0.20f, 80.0f, 0.6f);
}

#endif /* bed_h */
