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

    bool colorPaneActive;
    auto unicornBodyColor = PINK;
    auto unicornHornColor = GREEN;
    auto unicornManeColor = YELLOW;
    auto unicornTailColor = BLUE;
    std::vector<std::string> unicornParts = {"Body", "Mane", "Tail", "Horn"};
    std::string selectedPart = "Body";

    auto clicked = false;

    SetTargetFPS(50);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ONE)) colorPaneActive = !colorPaneActive;
//        if (IsKeyPressed(KEY_TWO)) selectedPart = unicornParts.;

        BeginDrawing();

        ClearBackground(WHITE);

        BeginMode3D(camera);
        const float radius = 10.0f;
        float camX = (float)sin(GetTime()) * radius;
        float camZ = (float)cos(GetTime()) * radius;
        camera.position = (Vector3){camX, radius, camZ};
        DrawModel(unicornBody, unicornBodyPosition, 1.0f, unicornBodyColor);
        DrawModel(unicornHorn, unicornBodyPosition, 1.0f, unicornHornColor);
        DrawModel(unicornMane, unicornBodyPosition, 1.0f, unicornManeColor);
        DrawModel(unicornTail, unicornBodyPosition, 1.0f, unicornTailColor);

        DrawGrid(4.0f, 8.0f);

        EndMode3D();

        if (colorPaneActive) {
            if (selectedPart == "Body"){
                unicornBodyColor = GuiColorPicker((Rectangle) {20, 100, 120, 120}, nullptr, unicornBodyColor);
            }
            if (selectedPart == "Horn"){
                unicornHornColor = GuiColorPicker((Rectangle) {20, 100, 120, 120}, nullptr, unicornHornColor);
            }
            if (selectedPart == "Mane"){
                unicornManeColor = GuiColorPicker((Rectangle) {20, 100, 120, 120}, nullptr, unicornManeColor);
            }
            if (selectedPart == "Tail"){
                unicornTailColor = GuiColorPicker((Rectangle) {20, 100, 120, 120}, nullptr, unicornTailColor);
            }
        }


        GuiDrawRectangle((Rectangle) {0, 0, 14, 240}, 1, GREEN, GREEN);
        GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
        GuiDrawRectangle((Rectangle) {306, 0, 320, 240}, 1, PURPLE, PURPLE);
        GuiDrawRectangle((Rectangle) {0, 0, 320, 14}, 1, YELLOW, YELLOW);
        DrawText("PUDGY", 142, 2, 10, BLACK);
        GuiDrawRectangle((Rectangle) {0, 226, 320, 240}, 1, RED, RED);
        DrawText("v0.01 alpha", 132, 228, 10, BLACK);

        if (GuiButton((Rectangle) {1, 2, 20, 20}, "B")){
            selectedPart = "Body";
            colorPaneActive = !colorPaneActive;
        }
        if (GuiButton((Rectangle) {299, 2, 20, 20}, "H")){
            selectedPart = "Horn";
            colorPaneActive = !colorPaneActive;
        }
//        if (GuiButton((Rectangle) {1, 105, 20, 20}, "<")) colorPaneActive = !colorPaneActive;
//        if (GuiButton((Rectangle) {299, 105, 20, 20}, ">")) colorPaneActive = !colorPaneActive;
        if (GuiButton((Rectangle) {1, 219, 20, 20}, "M")){
            selectedPart = "Mane";
            colorPaneActive = !colorPaneActive;
        }
        if (GuiButton((Rectangle) {299, 219, 20, 20}, "T")){
            selectedPart = "Tail";
            colorPaneActive = !colorPaneActive;
        }


        EndDrawing();
    }

    CloseWindow();

    return 0;
}