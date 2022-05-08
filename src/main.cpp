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

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 320;
    const int screenHeight = 240;

    InitWindow(screenWidth, screenHeight, "320 x 240");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 1.0f, 8.0f, 1.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    Model unicornBody = LoadModel("../src/include/unicorn_body.obj");
    Model unicornHorn = LoadModel("../src/include/unicorn_horn.obj");
    Model unicornMane = LoadModel("../src/include/unicorn_mane.obj");
    Model unicornTail = LoadModel("../src/include/unicorn_tail.obj");
    Vector3 unicornBodyPosition = {0.0f, 0.0f, 0.0f };
    Vector3 unicornHornPosition = {0.0f, 0.0f, 0.0f };
    Vector3 unicornManePosition = {0.0f, 0.0f, 0.0f };
    Vector3 unicornTailPosition = {0.0f, 0.0f, 0.0f };

    SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

    auto colorPaneActive = true;
    auto unicornBodyColor = BLUE;
    auto unicornHornColor = GREEN;
    auto unicornManeColor = PINK;
    auto unicornTailColor = YELLOW;
    RenderTexture2D target = LoadRenderTexture(320, 240);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (IsKeyPressed(KEY_ONE)) colorPaneActive = !colorPaneActive;

        BeginDrawing();

        ClearBackground(BLACK);

        // Render generated texture using selected postprocessing shader
//        BeginShaderMode(1);
        // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
//        DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2){ 0, 0 }, WHITE);
//        EndShaderMode();


        GuiWindowBox((Rectangle) {121, 0, 200, 240}, "PUDGY");

        BeginMode3D(camera);
//        rlViewport(121, 0, 200, 240);
        const float radius = 10.0f;
        float camX = (float)sin(GetTime()) * radius;
        float camZ = (float)cos(GetTime()) * radius;
        camera.position = (Vector3){camX, 8.0f, camZ};
        DrawModel(unicornBody, unicornBodyPosition, 1.0f, unicornBodyColor);
        DrawModel(unicornHorn, unicornBodyPosition, 1.0f, unicornHornColor);
        DrawModel(unicornMane, unicornBodyPosition, 1.0f, unicornManeColor);
        DrawModel(unicornTail, unicornBodyPosition, 1.0f, unicornTailColor);
        EndMode3D();

        if (colorPaneActive) {
            GuiDrawRectangle((Rectangle) {0, 0, 120, 120}, 1, BLACK, WHITE);
            DrawText(" Sofia's Pudgy Unicorn!", 2, 2, 10, GRAY);
            DrawText(" Sofia's Pudgy Unicorn!", 1, 1, 10, BLUE);
            unicornBodyColor = GuiColorPicker((Rectangle) {0, 120, 120, 120}, nullptr, unicornBodyColor);
        }
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}