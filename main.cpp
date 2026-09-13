//
//  main.cpp
//  GRAND BEDROOM — 3D Computer Graphics Lab Project
//
//  Built with GLFW + GLAD + GLM, following the same file layout used
//  throughout this project's course template (shader.h / camera.h /
//  basic_camera.h / pointLight.h / sphere.h): every "task" of the scene
//  lives in its own header, and main.cpp just wires them together.
//
//  PROJECT REQUIREMENTS CHECKLIST
//  --------------------------------------------------------------------
//  1) 3D transformation      -> every object below is drawn through a
//                                translate * rotate * scale model matrix
//                                (see room.h / bed.h / furniture.h / fan.h)
//                                and the whole scene can additionally be
//                                rotated/scaled live with the arrow keys.
//  2) Viewing transformation -> camera.h (Euler-angle FPS camera, mouse +
//                                WASD) and basic_camera.h (hand-built
//                                u/v/n view matrix). Press V to switch.
//  3) One moving object      -> fan.h: a ceiling fan whose blades keep
//                                spinning every frame (press F to pause).
//                                Bonus: furniture.h's wardrobe door slides
//                                open/closed (press G), and clock.h's wall
//                                clock hands sweep + its pendulum swings
//                                every frame too.
//  4) Two kinds of light     -> pointLight.h (chandelier + 2 bedside lamps
//                                + a floor lamp = 4 point lights) and
//                                spotLight.h (a desk reading lamp).
//  5) Different colors       -> every material below has its own distinct
//                                ambient/diffuse/specular color.
//  --------------------------------------------------------------------
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <algorithm>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "spotLight.h"
#include "cube.h"
#include "sphere.h"

#include "room.h"
#include "bed.h"
#include "furniture.h"
#include "fan.h"
#include "clock.h"
#include "decor.h"

using namespace std;

// ---------------------------------------------------------------------
// forward declarations
// ---------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);

// ---------------------------------------------------------------------
// settings
// ---------------------------------------------------------------------
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 800;

// ---------------------------------------------------------------------
// camera / viewing transformation
// ---------------------------------------------------------------------
Camera camera(glm::vec3(4.0f, 1.6f, 9.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool useBasicCamera = false;                 // V toggles Camera <-> BasicCamera
BasicCamera basic_camera(4.0f, 1.6f, 9.5f, 4.0f, 1.6f, 8.5f, glm::vec3(0.0f, 1.0f, 0.0f));

// ---------------------------------------------------------------------
// whole-scene 3D transformation demo (arrow keys)
// ---------------------------------------------------------------------
float sceneRotateY = 0.0f;
float sceneScale = 1.0f;

// ---------------------------------------------------------------------
// moving objects
// ---------------------------------------------------------------------
CeilingFan ceilingFan(glm::vec3(2.6f, ROOM_H - 0.05f, 3.0f));
WallClock wallClock(glm::vec3(ROOM_W - WALL_T - 0.17f, 2.85f, (BED_Z0 + BED_Z1) / 2.0f));
bool wardrobeOpen = false;
float doorOffset = 0.0f;   // animated toward 0 (closed) or 1 (open)

// ---------------------------------------------------------------------
// timing
// ---------------------------------------------------------------------
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// ---------------------------------------------------------------------
// LIGHT TYPE #1: point lights — chandelier, two bedside lamps, floor lamp
// ---------------------------------------------------------------------
PointLight pointlight1(
    CHANDELIER_POS.x, CHANDELIER_POS.y, CHANDELIER_POS.z,
    0.10f, 0.09f, 0.06f,      // ambient
    0.85f, 0.78f, 0.55f,      // diffuse (warm white)
    1.00f, 0.95f, 0.80f,      // specular
    1.0f, 0.045f, 0.0075f,    // attenuation
    1);
PointLight pointlight2(
    BEDSIDE_LAMP1_POS.x, BEDSIDE_LAMP1_POS.y, BEDSIDE_LAMP1_POS.z,
    0.06f, 0.05f, 0.02f,
    0.80f, 0.55f, 0.20f,      // diffuse (amber)
    0.70f, 0.55f, 0.25f,
    1.0f, 0.14f, 0.07f,
    2);
PointLight pointlight3(
    BEDSIDE_LAMP2_POS.x, BEDSIDE_LAMP2_POS.y, BEDSIDE_LAMP2_POS.z,
    0.06f, 0.05f, 0.02f,
    0.80f, 0.55f, 0.20f,      // diffuse (amber)
    0.70f, 0.55f, 0.25f,
    1.0f, 0.14f, 0.07f,
    3);
PointLight pointlight4(
    FLOOR_LAMP_LIGHT_POS.x, FLOOR_LAMP_LIGHT_POS.y, FLOOR_LAMP_LIGHT_POS.z,
    0.05f, 0.05f, 0.03f,
    0.55f, 0.48f, 0.28f,      // diffuse (soft gold)
    0.4f, 0.4f, 0.3f,
    1.0f, 0.09f, 0.032f,
    4);
PointLight* pointLights[4] = { &pointlight1, &pointlight2, &pointlight3, &pointlight4 };

// ---------------------------------------------------------------------
// LIGHT TYPE #2: spotlight — the study-desk reading lamp
// ---------------------------------------------------------------------
SpotLight deskSpotLight(
    DESK_LAMP_POS, DESK_LAMP_DIR,
    0.05f, 0.05f, 0.05f,      // ambient
    0.95f, 0.92f, 0.75f,      // diffuse (crisp reading-lamp white)
    1.0f, 1.0f, 1.0f,         // specular
    1.0f, 0.09f, 0.032f,      // attenuation
    16.0f, 24.0f,             // inner / outer cutoff angles (degrees)
    1);

int main()
{
    // -------------------------------------------------------------
    // glfw: initialize and configure
    // -------------------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Grand Bedroom - 3D Computer Graphics Lab Project", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // -------------------------------------------------------------
    // glad: load all OpenGL function pointers
    // -------------------------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // -------------------------------------------------------------
    // build and compile the shader programs
    // -------------------------------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    Shader unlitShader("vertexShader.vs", "fragmentShader.fs");

    // -------------------------------------------------------------
    // scene objects — one shared unit Cube and one shared unit Sphere
    // used (and re-transformed) for every piece of furniture
    // -------------------------------------------------------------
    Cube cube;
    Sphere sphere;

    std::cout <<
        "\n===================== CONTROLS =====================\n"
        " W / A / S / D   - move\n"
        " SPACE / LEFT-SHIFT - fly up / down\n"
        " Mouse            - look around\n"
        " V                - switch Camera <-> BasicCamera (viewing transform)\n"
        " F                - start/stop the ceiling fan (moving object)\n"
        " G                - open/close the wardrobe's sliding door\n"
        " 1 / 2 / 3 / 4    - toggle chandelier / bedside lamp L / R / floor lamp\n"
        " 5                - toggle the desk spotlight\n"
        " 9 / 0            - all lights ON / all lights OFF\n"
        " LEFT / RIGHT      - rotate whole scene (3D transform demo)\n"
        " UP / DOWN         - scale whole scene (3D transform demo)\n"
        " ESC               - quit\n"
        "=====================================================\n\n";

    // -------------------------------------------------------------
    // render loop
    // -------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // animate the moving objects ---------------------------------
        ceilingFan.update(deltaTime);
        wallClock.update(deltaTime);
        float doorTarget = wardrobeOpen ? 1.0f : 0.0f;
        doorOffset += (doorTarget - doorOffset) * std::min(1.0f, 4.0f * deltaTime);

        // render -------------------------------------------------------
        glClearColor(0.03f, 0.03f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.use();
        lightingShader.setVec3("viewPos", useBasicCamera ? basic_camera.eye : camera.Position);

        for (int i = 0; i < 4; i++)
            pointLights[i]->setUpPointLight(lightingShader);
        deskSpotLight.setUpSpotLight(lightingShader);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // ---- VIEWING TRANSFORMATION ----
        glm::mat4 view;
        if (useBasicCamera)
        {
            basic_camera.changeEye(camera.Position.x, camera.Position.y, camera.Position.z);
            glm::vec3 lookAtPoint = camera.Position + camera.Front;
            basic_camera.changeLookAt(lookAtPoint.x, lookAtPoint.y, lookAtPoint.z);
            basic_camera.changeViewUpVector(camera.Up);
            view = basic_camera.createViewMatrix();
        }
        else
        {
            view = camera.GetViewMatrix();
        }
        lightingShader.setMat4("view", view);

        // ---- whole-scene 3D transformation (rotate/scale demo) ----
        glm::vec3 roomCenter(ROOM_W / 2.0f, 0.0f, ROOM_D / 2.0f);
        glm::mat4 sceneModel = glm::translate(glm::mat4(1.0f), roomCenter);
        sceneModel = glm::rotate(sceneModel, glm::radians(sceneRotateY), glm::vec3(0.0f, 1.0f, 0.0f));
        sceneModel = glm::scale(sceneModel, glm::vec3(sceneScale));
        sceneModel = glm::translate(sceneModel, -roomCenter);

        // ---- draw the room and every piece of furniture ----
        buildRoom(cube, lightingShader, sceneModel);
        buildBed(cube, sphere, lightingShader, sceneModel);
        buildWardrobe(cube, lightingShader, sceneModel, doorOffset);
        buildStudyDesk(cube, lightingShader, sceneModel);
        buildBookshelf(cube, lightingShader, sceneModel);
        buildChandelier(cube, sphere, lightingShader, sceneModel);
        buildNightstand(cube, sphere, lightingShader, sceneModel, NIGHTSTAND1_POS, BEDSIDE_LAMP1_POS);
        buildNightstand(cube, sphere, lightingShader, sceneModel, NIGHTSTAND2_POS, BEDSIDE_LAMP2_POS);
        buildFloorLamp(cube, sphere, lightingShader, sceneModel);
        buildDeskLampFixture(cube, sphere, lightingShader, sceneModel);
        buildPictureFrame(cube, lightingShader, sceneModel);
        ceilingFan.draw(cube, sphere, lightingShader, sceneModel);
        wallClock.draw(cube, sphere, lightingShader, sceneModel);

        // ---- unlit "bulb" markers so every light source is visible ----
        unlitShader.use();
        unlitShader.setMat4("projection", projection);
        unlitShader.setMat4("view", view);
        auto drawBulb = [&](glm::vec3 pos, glm::vec3 color, bool on)
            {
                unlitShader.setVec3("color", on ? color : glm::vec3(0.12f, 0.12f, 0.12f));
                glm::mat4 model = glm::translate(sceneModel, pos);
                model = glm::scale(model, glm::vec3(0.055f));
                sphere.drawSphere(unlitShader, model);
            };
        drawBulb(CHANDELIER_POS, glm::vec3(1.0f, 0.92f, 0.70f), pointlight1.isOn());
        drawBulb(BEDSIDE_LAMP1_POS, glm::vec3(1.0f, 0.75f, 0.30f), pointlight2.isOn());
        drawBulb(BEDSIDE_LAMP2_POS, glm::vec3(1.0f, 0.75f, 0.30f), pointlight3.isOn());
        drawBulb(FLOOR_LAMP_LIGHT_POS, glm::vec3(1.0f, 0.85f, 0.55f), pointlight4.isOn());
        drawBulb(DESK_LAMP_POS, glm::vec3(1.0f, 0.95f, 0.80f), deskSpotLight.isOn());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// ---------------------------------------------------------------------
// continuous key polling: camera movement
// ---------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    // whole-scene 3D transformation demo
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        sceneRotateY -= 40.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        sceneRotateY += 40.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        sceneScale = std::min(1.3f, sceneScale + 0.3f * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        sceneScale = std::max(0.7f, sceneScale - 0.3f * deltaTime);
}

// ---------------------------------------------------------------------
// discrete key presses: toggles
// ---------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;

    if (key == GLFW_KEY_V)
        useBasicCamera = !useBasicCamera;

    if (key == GLFW_KEY_F)
        ceilingFan.toggle();

    if (key == GLFW_KEY_G)
        wardrobeOpen = !wardrobeOpen;

    if (key == GLFW_KEY_1) { pointlight1.isOn() ? pointlight1.turnOff() : pointlight1.turnOn(); }
    if (key == GLFW_KEY_2) { pointlight2.isOn() ? pointlight2.turnOff() : pointlight2.turnOn(); }
    if (key == GLFW_KEY_3) { pointlight3.isOn() ? pointlight3.turnOff() : pointlight3.turnOn(); }
    if (key == GLFW_KEY_4) { pointlight4.isOn() ? pointlight4.turnOff() : pointlight4.turnOn(); }
    if (key == GLFW_KEY_5) { deskSpotLight.isOn() ? deskSpotLight.turnOff() : deskSpotLight.turnOn(); }

    if (key == GLFW_KEY_9)
    {
        pointlight1.turnOn(); pointlight2.turnOn(); pointlight3.turnOn(); pointlight4.turnOn();
        deskSpotLight.turnOn();
    }
    if (key == GLFW_KEY_0)
    {
        pointlight1.turnOff(); pointlight2.turnOff(); pointlight3.turnOff(); pointlight4.turnOff();
        deskSpotLight.turnOff();
    }
}

// ---------------------------------------------------------------------
// glfw callbacks
// ---------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}