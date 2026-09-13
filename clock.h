//
//  clock.h
//  Grand Bedroom 3D Scene
//
//  BONUS moving object: an old-fashioned Vienna-regulator-style pendulum
//  wall clock — a tall wooden case with a decorative pediment and
//  finials, a round face with hour/minute hands that keep turning, and a
//  glass-fronted lower case where a brass pendulum bob swings side to
//  side. Two independent continuous animations (hands + pendulum), both
//  driven by update(deltaTime), exactly like fan.h's spinning blades.
//
//  The clock is mounted on a wall whose normal points along -X (a wall
//  standing in a Y-Z plane, like this room's right-hand wall behind the
//  bed's headboard) — "position" is the centre of the round dial.
//

#ifndef clock_h
#define clock_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "cube.h"
#include "sphere.h"

class WallClock
{
public:
    WallClock(glm::vec3 facePos, float faceRadius = 0.22f)
        : position(facePos), radius(faceRadius), minuteAngle(0.0f), hourAngle(0.0f), swingTime(0.0f) {
    }

    void update(float deltaTime)
    {
        minuteAngle += 36.0f * deltaTime;   // one full sweep every 10s
        hourAngle += 3.0f * deltaTime;      // twelve times slower
        swingTime += deltaTime;
        if (minuteAngle > 360.0f) minuteAngle -= 360.0f;
        if (hourAngle > 360.0f) hourAngle -= 360.0f;
    }

    void draw(const Cube& cube, const Sphere& sphere, Shader& shader, const glm::mat4& sceneModel) const
    {
        float caseHalfW = radius + 0.10f;          // half-width of the wooden case
        float caseDepth = 0.15f;                   // how far the case sticks out from the wall
        // NOTE on X convention: `position.x` is the dial's face (closest to
        // the viewer, standing just off the wall); the case body extends
        // *away* from the viewer, toward +X, back to the wall it is
        // mounted on. Anything meant to sit proud of the case front
        // (glass pane, keyhole) is placed at a slightly smaller x than
        // `position.x`; anything inside/behind the case uses position.x
        // + a fraction of caseDepth.
        float caseCenterX = position.x + caseDepth * 0.5f;
        float faceTop = position.y + radius;
        float faceBottom = position.y - radius;
        float caseTop = faceTop + 0.10f;
        float caseBottom = faceBottom - 0.85f;      // long lower case for the pendulum window

        // ---- Wooden case body (walnut) ------------------------------
        box(cube, shader, sceneModel, position.x, caseBottom, position.z - caseHalfW,
            caseDepth, caseTop - caseBottom, caseHalfW * 2.0f, 0.28f, 0.15f, 0.07f, 22.0f, 0.2f);

        // Side pillars — slightly darker trim down the front edges of the case
        box(cube, shader, sceneModel, position.x - 0.012f, caseBottom, position.z - caseHalfW - 0.012f,
            0.03f, caseTop - caseBottom, 0.05f, 0.20f, 0.10f, 0.05f, 22.0f, 0.2f);
        box(cube, shader, sceneModel, position.x - 0.012f, caseBottom, position.z + caseHalfW - 0.038f,
            0.03f, caseTop - caseBottom, 0.05f, 0.20f, 0.10f, 0.05f, 22.0f, 0.2f);

        // ---- Stepped cornice + small rounded arch on top, plus a crest
        // ornament — built from stacked boxes (matching the case's own
        // rectilinear style) rather than one oversized dome. ----------
        float cornice1H = 0.05f, cornice2H = 0.05f;
        box(cube, shader, sceneModel, position.x - 0.03f, caseTop, position.z - caseHalfW - 0.04f,
            caseDepth + 0.06f, cornice1H, (caseHalfW + 0.04f) * 2.0f, 0.20f, 0.10f, 0.05f, 22.0f, 0.2f);
        float step2Top = caseTop + cornice1H;
        box(cube, shader, sceneModel, position.x - 0.015f, step2Top, position.z - caseHalfW - 0.015f,
            caseDepth + 0.03f, cornice2H, (caseHalfW + 0.015f) * 2.0f, 0.24f, 0.12f, 0.06f, 22.0f, 0.2f);
        float archBaseY = step2Top + cornice2H;

        glm::mat4 pediment = glm::translate(glm::mat4(1.0f), glm::vec3(caseCenterX, archBaseY, position.z));
        pediment = glm::scale(pediment, glm::vec3(caseDepth * 0.55f, 0.09f, caseHalfW * 0.95f));
        sphere.drawSphere(shader, sceneModel * pediment, glm::vec3(0.16f, 0.08f, 0.04f), glm::vec3(0.30f, 0.16f, 0.08f), glm::vec3(0.25f), 20.0f);

        glm::mat4 crest = glm::translate(glm::mat4(1.0f), glm::vec3(caseCenterX, archBaseY + 0.10f, position.z));
        crest = glm::scale(crest, glm::vec3(0.04f));
        sphere.drawSphere(shader, sceneModel * crest, glm::vec3(0.16f, 0.08f, 0.04f), glm::vec3(0.30f, 0.16f, 0.08f), glm::vec3(0.3f), 30.0f);

        // Two turned-wood finials at the top corners, sitting on the cornice
        for (int s = -1; s <= 1; s += 2)
        {
            glm::vec3 fpos(position.x + caseDepth * 0.3f, step2Top, position.z + s * (caseHalfW - 0.02f));
            glm::mat4 finialBase = glm::translate(glm::mat4(1.0f), fpos);
            finialBase = glm::scale(finialBase, glm::vec3(0.035f));
            sphere.drawSphere(shader, sceneModel * finialBase, glm::vec3(0.16f, 0.08f, 0.04f), glm::vec3(0.30f, 0.16f, 0.08f), glm::vec3(0.25f), 20.0f);
            glm::mat4 finialTip = glm::translate(glm::mat4(1.0f), fpos + glm::vec3(0.0f, 0.05f, 0.0f));
            finialTip = glm::scale(finialTip, glm::vec3(0.018f));
            sphere.drawSphere(shader, sceneModel * finialTip, glm::vec3(0.16f, 0.08f, 0.04f), glm::vec3(0.30f, 0.16f, 0.08f), glm::vec3(0.25f), 20.0f);
        }

        // ---- Lower glass-fronted window revealing the pendulum -------
        box(cube, shader, sceneModel, position.x - 0.012f, caseBottom + 0.06f, position.z - (caseHalfW - 0.05f),
            0.012f, (faceBottom - 0.05f) - (caseBottom + 0.06f), (caseHalfW - 0.05f) * 2.0f, 0.10f, 0.11f, 0.15f, 130.0f, 0.8f);

        // Small brass keyhole/latch below the glass, for detail
        box(cube, shader, sceneModel, position.x - 0.02f, caseBottom + 0.10f, position.z - 0.012f,
            0.02f, 0.03f, 0.024f, 0.80f, 0.65f, 0.22f, 90.0f, 0.7f);

        // Bottom drop finial under the case
        glm::mat4 drop = glm::translate(glm::mat4(1.0f), glm::vec3(caseCenterX, caseBottom - 0.02f, position.z));
        drop = glm::scale(drop, glm::vec3(0.045f, 0.06f, 0.045f));
        sphere.drawSphere(shader, sceneModel * drop, glm::vec3(0.16f, 0.08f, 0.04f), glm::vec3(0.30f, 0.16f, 0.08f), glm::vec3(0.25f), 20.0f);

        // ---- Brass bezel (a thin ring — kept almost flat so its own
        // curvature never competes with the face dome in front of it)
        // + cream dial ---------------------------------------------------
        glm::mat4 bezel = glm::translate(glm::mat4(1.0f), position);
        bezel = glm::scale(bezel, glm::vec3(0.014f, radius + 0.03f, radius + 0.03f));
        sphere.drawSphere(shader, sceneModel * bezel, glm::vec3(0.35f, 0.28f, 0.10f), glm::vec3(0.75f, 0.60f, 0.22f), glm::vec3(0.8f), 90.0f);

        // Face sits well forward of the bezel and is noticeably thicker,
        // so its dome is guaranteed to be in front of the bezel's dome
        // everywhere across the dial, not just at the very centre.
        glm::mat4 face = glm::translate(glm::mat4(1.0f), position + glm::vec3(-0.035f, 0.0f, 0.0f));
        face = glm::scale(face, glm::vec3(0.04f, radius, radius));
        sphere.drawSphere(shader, sceneModel * face, glm::vec3(0.34f, 0.32f, 0.27f), glm::vec3(0.95f, 0.93f, 0.85f), glm::vec3(0.4f), 40.0f);

        // 12 tick marks around the dial (4 slightly bigger "hour" marks) —
        // pushed further forward again so they read clearly on the face
        for (int i = 0; i < 12; i++)
        {
            float a = glm::radians(30.0f * i);
            bool major = (i % 3 == 0);
            glm::vec3 tickPos = position + glm::vec3(-0.085f, cosf(a) * (radius - 0.045f), sinf(a) * (radius - 0.045f));
            glm::mat4 tick = glm::translate(glm::mat4(1.0f), tickPos);
            tick = glm::scale(tick, glm::vec3(0.01f, major ? 0.032f : 0.018f, major ? 0.032f : 0.018f));
            cube.drawCubeRGB(shader, sceneModel * tick, 0.12f, 0.10f, 0.08f, 15.0f, 0.05f);
        }

        // ---- Hour + minute hands, and the centre pin (frontmost) ------
        drawHand(cube, shader, sceneModel, hourAngle, radius * 0.42f, 0.024f, glm::vec3(0.08f, 0.08f, 0.08f));
        drawHand(cube, shader, sceneModel, minuteAngle, radius * 0.68f, 0.017f, glm::vec3(0.10f, 0.10f, 0.10f));

        glm::mat4 pin = glm::translate(glm::mat4(1.0f), position + glm::vec3(-0.105f, 0.0f, 0.0f));
        pin = glm::scale(pin, glm::vec3(0.018f));
        sphere.drawSphere(shader, sceneModel * pin, glm::vec3(0.05f), glm::vec3(0.1f), glm::vec3(0.2f), 20.0f);

        // ---- Swinging brass pendulum, visible through the glass -------
        float swingAngle = 14.0f * sinf(swingTime * 2.4f);   // +-14 degrees, ticking back and forth
        float rodLength = 0.38f;
        glm::vec3 pivot = glm::vec3(caseCenterX, faceBottom - 0.06f, position.z);
        glm::mat4 rod = glm::translate(glm::mat4(1.0f), pivot);
        rod = glm::rotate(rod, glm::radians(swingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rodModel = glm::translate(rod, glm::vec3(-0.006f, -rodLength, -0.006f));
        rodModel = glm::scale(rodModel, glm::vec3(0.012f, rodLength, 0.012f));
        cube.drawCubeRGB(shader, sceneModel * rodModel, 0.75f, 0.60f, 0.22f, 70.0f, 0.5f);

        glm::mat4 bob = glm::translate(rod, glm::vec3(0.0f, -rodLength, 0.0f));
        bob = glm::scale(bob, glm::vec3(0.03f, 0.13f, 0.13f));
        sphere.drawSphere(shader, sceneModel * bob, glm::vec3(0.35f, 0.28f, 0.10f), glm::vec3(0.80f, 0.65f, 0.24f), glm::vec3(0.9f), 100.0f);
    }

private:
    inline void box(const Cube& cube, Shader& shader, const glm::mat4& sceneModel,
        float x, float y, float z, float sx, float sy, float sz,
        float r, float g, float b, float shininess, float spec) const
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
        model = glm::scale(model, glm::vec3(sx, sy, sz));
        cube.drawCubeRGB(shader, sceneModel * model, r, g, b, shininess, spec);
    }

    // A hand is a thin bar that starts at the clock's centre and points
    // toward 12 o'clock (world +Y) when angle == 0; rotating it about the
    // face's normal axis (world X, since the wall faces -X) sweeps it
    // around the dial exactly like a real clock hand.
    void drawHand(const Cube& cube, Shader& shader, const glm::mat4& sceneModel, float angleDeg, float length, float thickness, glm::vec3 color) const
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position + glm::vec3(-0.095f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-angleDeg), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -thickness / 2.0f));
        model = glm::scale(model, glm::vec3(0.012f, length, thickness));
        cube.drawCubeRGB(shader, sceneModel * model, color.r, color.g, color.b, 10.0f, 0.05f);
    }

    glm::vec3 position;
    float radius;
    float minuteAngle;
    float hourAngle;
    float swingTime;
};

#endif /* clock_h */