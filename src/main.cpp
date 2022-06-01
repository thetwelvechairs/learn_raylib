#include "raylib.h"
#include <cstdlib>
#include <iostream>


#define RAYGUI_IMPLEMENTATION

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

#include "include/raygui.h"
#include "rlgl.h"
#include <random>


[[maybe_unused]] static int randomInteger(int to, int from){
    std::random_device randomizerSeed;
    std::default_random_engine randomEngine(randomizerSeed());
    std::uniform_int_distribution<int> randomRange(from, to);
    return randomRange(randomEngine);
}

[[maybe_unused]] static float randomFloat(float to, float from){
    std::random_device randomizerSeed;
    std::default_random_engine randomEngine(randomizerSeed());
    std::uniform_real_distribution<float> distribution(from, to);
    return distribution(randomEngine);
}

[[maybe_unused]] int getNext(const std::vector<int>& vect, size_t i, auto v) {
    return vect[(i + 1) % v.size()];
}

auto rayHit(Camera camera, Ray ray, Model model){
    ray = GetMouseRay(GetMousePosition(), camera);
    return GetRayCollisionModel(ray, model);
}

int main(){
    const int screenWidth = 320;
    const int screenHeight = 240;

    std::string title = std::to_string(screenWidth) + "x" + std::to_string(screenHeight);
    InitWindow(screenWidth, screenHeight, title.c_str());
    std::vector<Model> models;
    Model unicornBody = LoadModel("../src/include/unicorn_body.obj");
    models.push_back(unicornBody);
    Model unicornHorn = LoadModel("../src/include/unicorn_horn.obj");
    models.push_back(unicornHorn);
    Model unicornMane = LoadModel("../src/include/unicorn_mane.obj");
    models.push_back(unicornMane);
    Model unicornTail = LoadModel("../src/include/unicorn_tail.obj");
    models.push_back(unicornTail);

    Vector3 unicornBodyPosition = {0.0f, 0.0f, 0.0f};

    // Define the camera to look into our 3d world
    Camera camera = {0};
    auto cameraHeight = 2.0f;
    camera.position = (Vector3){0.0f, cameraHeight, 0.0f};
    camera.target = (Vector3){0.0f, cameraHeight, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 35.0f;
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    SetCameraMode(camera, CAMERA_ORBITAL);

    bool colorPaneActive = false;
    auto unicornBodyColor = PINK;
    auto unicornHornColor = GREEN;
    auto unicornManeColor = YELLOW;
    auto unicornTailColor = BLUE;

    bool exitWindow = false;

    Ray ray = {0};
    RayCollision collision = {false};

    int collisionModelIndex = -1;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    while (!IsKeyPressed(KEY_ESCAPE)){
        BeginDrawing();

        ClearBackground(WHITE);

        BeginMode3D(camera);
        const float radius = 10.0f;
        const float speedFactor = 4.0f;
        float camX = (float)sin(GetTime() / speedFactor) * radius;
        float camZ = (float)cos(GetTime() / speedFactor) * radius;
        camera.position = (Vector3){camX, cameraHeight, camZ};
        DrawModel(unicornBody, unicornBodyPosition, 1.0f, unicornBodyColor);
        DrawModel(unicornHorn, unicornBodyPosition, 1.0f, unicornHornColor);
        DrawModel(unicornMane, unicornBodyPosition, 1.0f, unicornManeColor);
        DrawModel(unicornTail, unicornBodyPosition, 1.0f, unicornTailColor);

        DrawGrid(14.0f, 0.5f);

        EndMode3D();

        if (!colorPaneActive && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            collisionModelIndex = -1;
            for (int n = 0; n < models.size(); n++) {
                collision = rayHit(camera, ray, models.at(n));
                if (collision.hit) {
                    collisionModelIndex = n;
                    colorPaneActive = true;
                    exitWindow = false;
                }
            }
        }

        if (!exitWindow && collisionModelIndex > -1){
            if (collisionModelIndex == 0) {
                exitWindow = GuiWindowBox((Rectangle) {60, 40, 160, 160}, "Body");
                unicornBodyColor = GuiColorPicker((Rectangle) {70, 70, 120, 120}, nullptr, unicornBodyColor);
            } else if (collisionModelIndex == 1) {
                exitWindow = GuiWindowBox((Rectangle) {60, 40, 160, 160}, "Hair");
                unicornHornColor = GuiColorPicker((Rectangle) {70, 70, 120, 120}, nullptr, unicornHornColor);
            } else if (collisionModelIndex == 2) {
                exitWindow = GuiWindowBox((Rectangle) {60, 40, 160, 160}, "Mane");
                unicornManeColor = GuiColorPicker((Rectangle) {70, 70, 120, 120}, nullptr, unicornManeColor);
            } else if (collisionModelIndex == 3) {
                exitWindow = GuiWindowBox((Rectangle) {60, 40, 160, 160}, "Tail");
                unicornTailColor = GuiColorPicker((Rectangle) {70, 70, 120, 120}, nullptr, unicornTailColor);
            }
        }
        else{
            colorPaneActive = false;
        }

        EndDrawing();
        std::cout << "============================" << std::endl;
        std::cout << "| colorPaneActive:     " << colorPaneActive << std::endl;
        std::cout << "| exitWindow:          " << exitWindow << std::endl;
        std::cout << "| collisionModelIndex: " << collisionModelIndex << std::endl;
    }

    CloseWindow();

    return 0;
}