# Grand Bedroom — 3D Computer Graphics Lab Project

A textured-free, fully lit 3D bedroom scene built with **GLFW + GLAD + GLM**
(modern OpenGL 3.3 core, shader-based pipeline) — same libraries, same file
layout, and the same `Shader` / `Camera` / `BasicCamera` / `PointLight`
helper classes as the course template used in
[sabbir2667/Amimaiton-lab-pro](https://github.com/sabbir2667/Amimaiton-lab-pro).

Rather than one giant `main.cpp`, the scene is split into one file per task,
all `#include`d from `main.cpp`, exactly like the reference repo:

```
main.cpp                        window/GLFW setup, camera, lights, render loop
shader.h                        Shader class (compiles/links .vs + .fs, sets uniforms)
camera.h                        Euler-angle FPS camera            -> VIEWING TRANSFORMATION #1
basic_camera.h                  hand-built u/v/n view matrix       -> VIEWING TRANSFORMATION #2
pointLight.h                    point-light class                  -> LIGHT TYPE #1
spotLight.h                     spotlight class                    -> LIGHT TYPE #2
cube.h                          reusable unit cube (position+normal VAO)
sphere.h                        reusable unit sphere (position+normal VAO)
room.h                          floor / walls / ceiling / window / curtains
bed.h                           bed frame, mattress, headboard, pillows, blanket
furniture.h                     wardrobe (sliding door), study desk + chair, bookshelf
fan.h                           ceiling fan                        -> MOVING OBJECT
decor.h                         chandelier, lamps, picture frame; light-fixture positions
vertexShader.vs / fragmentShader.fs                     unlit "bulb" marker shader
vertexShaderForPhongShading.vs / fragmentShaderForPhongShading.fs   per-pixel Phong lighting
third_party/glad/                vendored OpenGL 3.3 core loader (no separate install needed)
CMakeLists.txt                   cross-platform build script
```

## Requirements checklist

| # | Requirement | Where |
|---|---|---|
| 1 | 3D transformation | Every object is drawn with a `translate * rotate * scale` model matrix (see any `build...()` function in `room.h` / `bed.h` / `furniture.h` / `fan.h`). The whole scene can also be rotated/scaled live with the arrow keys (`main.cpp`, `sceneModel`). |
| 2 | Viewing transformation | `camera.h` (`glm::lookAt`-based FPS camera, mouse + WASD) **and** `basic_camera.h` (view matrix built by hand from the *u, v, n* basis vectors). Press **V** to switch between the two live. |
| 3 | One moving object | `fan.h` — a ceiling fan whose blades keep spinning every frame (`angle += speed * deltaTime`). Press **F** to pause/resume it. *Bonus second moving object:* the wardrobe's sliding door in `furniture.h` (press **G**). |
| 4 | Two kinds of light | **Point lights** (`pointLight.h`): chandelier + 2 bedside lamps + a floor lamp. **Spotlight** (`spotLight.h`): the study-desk reading lamp, with a soft-edged cone (inner/outer cutoff). |
| 5 | Different colors | Every material below (walls, bed linens, wardrobe, curtains, rug, books, lamps…) has its own distinct ambient/diffuse/specular color — see the `drawCubeRGB(...)` calls throughout `room.h`/`bed.h`/`furniture.h`/`decor.h`. |

## Controls

```
 W / A / S / D        move
 SPACE / LEFT-SHIFT   fly up / down
 Mouse                look around
 V                     switch Camera <-> BasicCamera (viewing transform demo)
 F                     start/stop the ceiling fan (moving object)
 G                     open/close the wardrobe's sliding door
 1 / 2 / 3 / 4         toggle chandelier / bedside lamp L / bedside lamp R / floor lamp
 5                     toggle the desk spotlight
 9 / 0                 all lights ON / all lights OFF
 LEFT / RIGHT          rotate the whole scene   (3D transform demo)
 UP / DOWN             scale the whole scene    (3D transform demo)
 ESC                   quit
```

## Building

### Option A — CMake (Linux / macOS / Windows, recommended)

Everything needed to load OpenGL (GLAD) is already vendored in
`third_party/glad/`, so you only need GLFW and GLM on your system.

```bash
# Debian/Ubuntu
sudo apt install libglfw3-dev libglm-dev libgl1-mesa-dev

# macOS (Homebrew)
brew install glfw glm

# then, from the project folder:
cmake -S . -B build
cmake --build build -j
./build/GrandBedroomScene        # (or build/Debug/GrandBedroomScene.exe on Windows)
```

On Windows with [vcpkg](https://vcpkg.io/):

```
vcpkg install glfw3 glm
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=<vcpkg root>/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

### Option B — Visual Studio, same workflow as the reference repo

The reference repo's `.sln`/`.vcxproj` set up include/lib paths for GLFW,
GLM and GLAD via vcpkg. To reuse that exact workflow:

1. `vcpkg install glfw3 glm` (and integrate vcpkg with `vcpkg integrate install`).
2. Create a new empty C++ console project named e.g. `GrandBedroomScene`.
3. Add every `.cpp`/`.h` file from this folder to the project, **plus**
   `third_party/glad/src/glad.c` and add `third_party/glad/include` to
   *Additional Include Directories* (or `vcpkg install glad` instead of the
   vendored copy, exactly like the reference repo does).
4. Set the project's *Working Directory* (Debugging settings) to this
   folder so the `.vs`/`.fs` shader files can be found at runtime with
   their relative paths — or copy the shader files next to the built
   `.exe`, same as the reference repo's post-build behaviour.
5. Build & run (x64, Debug or Release).

The program has been test-built and test-run headlessly on Linux
(g++ 13, GLFW 3.3, Mesa software rendering) to confirm every file compiles
and the scene renders correctly before delivery.
