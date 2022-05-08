/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

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
    camera.position = (Vector3){ 1.0f, 5.0f, 1.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    Model unicornBody = LoadModel("../src/include/unicorn.obj");                 // Load OBJ model
    Vector3 unicornBodyPosition = {0.0f, 0.0f, 0.0f };

    SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

    auto mainColor = PINK;
    RenderTexture2D target = LoadRenderTexture(320, 240);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(DARKGRAY);

        // Render generated texture using selected postprocessing shader
//        BeginShaderMode(1);
        // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
//        DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2){ 0, 0 }, WHITE);
//        EndShaderMode();

        BeginMode3D(camera);
        const float radius = 10.0f;
        float camX = (float)sin(GetTime()) * radius;
        float camZ = (float)cos(GetTime()) * radius;
        camera.position = (Vector3){camX, 5.0f, camZ};
//        rlScalef(1.2f, 1.2f, 1.2f);
        DrawModel(unicornBody, unicornBodyPosition, 1.0f, mainColor);
        EndMode3D();
        GuiDrawRectangle((Rectangle){0, 0, 120, 120}, 1, BLACK, LIGHTGRAY);
        DrawText(" Sofia's Pudgy Unicorn!", 2, 2, 11, GRAY);
        DrawText(" Sofia's Pudgy Unicorn!", 1, 1, 11, BLUE);
        mainColor = GuiColorPicker((Rectangle){0, 120, 120, 120}, nullptr, mainColor);

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}