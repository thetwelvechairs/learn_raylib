#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
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

int getNext(std::vector<int> vect, size_t i, auto v) {
    return vect[ (i+1) % v.size() ];
}

int main()
{
    const int screenWidth = 320;
    const int screenHeight = 240;

    std::string title = std::to_string(screenWidth) + "x" + std::to_string(screenHeight);
    InitWindow(screenWidth, screenHeight, title.c_str());

    Model unicornBody = LoadModel("../src/include/unicorn_body.obj");
    Model unicornHorn = LoadModel("../src/include/unicorn_horn.obj");
    Model unicornMane = LoadModel("../src/include/unicorn_mane.obj");
    Model unicornTail = LoadModel("../src/include/unicorn_tail.obj");
    Vector3 unicornBodyPosition = {0.0f, 0.0f, 0.0f };

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 8.0f, 0.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 55.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    SetCameraMode(camera, CAMERA_ORBITAL);

    auto colorPaneActive = false;
    auto unicornBodyColor = BLUE;
    auto unicornHornColor = GREEN;
    auto unicornManeColor = YELLOW;
    auto unicornTailColor = PINK;
    std::vector<std::string> unicornParts = {"Body", "Mane", "Tail", "Horn"};
    std::string selectedPart = "Body";

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ONE)) colorPaneActive = !colorPaneActive;
//        if (IsKeyPressed(KEY_TWO)) selectedPart = unicornParts.;

        BeginDrawing();

        ClearBackground(DARKGRAY);

        BeginMode3D(camera);
        const float radius = 8.0f;
        float camX = (float)sin(GetTime()) * radius;
        float camZ = (float)cos(GetTime()) * radius;
        camera.position = (Vector3){camX, 8.0f, camZ};
        DrawModel(unicornBody, unicornBodyPosition, 1.0f, unicornBodyColor);
        DrawModel(unicornHorn, unicornBodyPosition, 1.0f, unicornHornColor);
        DrawModel(unicornMane, unicornBodyPosition, 1.0f, unicornManeColor);
        DrawModel(unicornTail, unicornBodyPosition, 1.0f, unicornTailColor);

        DrawGrid(60, 2.0f);

        EndMode3D();

        if (colorPaneActive) {
            unicornBodyColor = GuiColorPicker((Rectangle) {0, 120, 120, 120}, nullptr, unicornBodyColor);
        }

        GuiDrawRectangle((Rectangle) {0, 0, 10, 240}, 1, BLACK, LIGHTGRAY);
        DrawText(">", 4, 110, 10, BLACK);
        GuiDrawRectangle((Rectangle) {0, 0, 320, 20}, 1, BLACK, LIGHTGRAY);
        DrawText("PUDGY v0.01 alpha", 5, 5, 10, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}