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

[[maybe_unused]] int getNext(const std::vector<int>& vect, size_t i, auto v) {
    return vect[ (i+1) % v.size() ];
}

int main(){
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
    Camera camera = {0};
    auto cameraHeight = 2.0f;
    camera.position = (Vector3){ 0.0f, cameraHeight, 0.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, cameraHeight, 0.0f };      // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};          // Camera up vector (rotation towards target)
    camera.fovy = 35.0f;                                // Camera field-of-view Y
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

    Vector3 cubePosition = { 0.0f, 1.0f, 0.0f };
    Vector3 cubeSize = { 2.0f, 2.0f, 2.0f };
    Ray ray = { 0 };                    // Picking line ray
    RayCollision collision = { false };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ONE)) colorPaneActive = !colorPaneActive;
//        if (IsKeyPressed(KEY_TWO)) selectedPart = unicornParts.;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (!collision.hit)
            {
                ray = GetMouseRay(GetMousePosition(), camera);

                // Check collision between ray and box
                collision = GetRayCollisionBox(ray,
                                               (BoundingBox){(Vector3){ cubePosition.x - cubeSize.x/2, cubePosition.y - cubeSize.y/2, cubePosition.z - cubeSize.z/2 },
                                                             (Vector3){ cubePosition.x + cubeSize.x/2, cubePosition.y + cubeSize.y/2, cubePosition.z + cubeSize.z/2 }});
            }
            else collision.hit = false;
        }

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

        if (collision.hit){
//            DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, RED);
            DrawCubeWires(unicornBodyPosition, cubeSize.x, cubeSize.y, cubeSize.z, MAROON);
            DrawCubeWires(unicornBodyPosition, cubeSize.x + 0.2f, cubeSize.y + 0.2f, cubeSize.z + 0.2f, GREEN);
        }
        else{
//            DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, GRAY);
//            DrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, DARKGRAY);
        }

        EndMode3D();

        if (colorPaneActive){
            if (selectedPart == "Body"){
                unicornBodyColor = GuiColorPicker((Rectangle) {170, 110, 120, 120}, nullptr, unicornBodyColor);
            }
            if (selectedPart == "Horn"){
                unicornHornColor = GuiColorPicker((Rectangle) {170, 110, 120, 120}, nullptr, unicornHornColor);
            }
            if (selectedPart == "Mane"){
                unicornManeColor = GuiColorPicker((Rectangle) {170, 110, 120, 120}, nullptr, unicornManeColor);
            }
            if (selectedPart == "Tail"){
                unicornTailColor = GuiColorPicker((Rectangle) {170, 110, 120, 120}, nullptr, unicornTailColor);
            }
        }


//        GuiDrawRectangle((Rectangle) {0, 0, 14, 240}, 1, GREEN, GREEN);
        GuiSetStyle(BUTTON, BORDER_WIDTH, 2);
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
//        GuiDrawRectangle((Rectangle) {306, 0, 320, 240}, 1, PURPLE, PURPLE);
//        GuiDrawRectangle((Rectangle) {0, 0, 320, 14}, 1, YELLOW, YELLOW);
//        DrawText("PUDGY", 142, 2, 10, BLACK);
//        GuiDrawRectangle((Rectangle) {0, 226, 320, 240}, 1, RED, RED);
//        DrawText("v0.01 alpha", 132, 228, 10, BLACK);

        if (GuiButton((Rectangle) {0, 1, 50, 20}, "Body")){
            selectedPart = "Body";
            colorPaneActive = !colorPaneActive;
        }
        if (GuiButton((Rectangle) {0, 62, 50, 20}, "Horn")){
            selectedPart = "Horn";
            colorPaneActive = !colorPaneActive;
        }
//        if (GuiButton((Rectangle) {1, 105, 20, 20}, "<")) colorPaneActive = !colorPaneActive;
//        if (GuiButton((Rectangle) {299, 105, 20, 20}, ">")) colorPaneActive = !colorPaneActive;
        if (GuiButton((Rectangle) {0, 122, 50, 20}, "Tail")){
            selectedPart = "Tail";
            colorPaneActive = !colorPaneActive;
        }
        if (GuiButton((Rectangle) {0, 186, 50, 20}, "Mane")){
            selectedPart = "Mane";
            colorPaneActive = !colorPaneActive;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}