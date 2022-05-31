#include "raylib.h"
#include <cstdlib>

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
    return vect[ (i+1) % v.size() ];
}

auto rayHit(Camera camera, Ray ray, Model model){
    ray = GetMouseRay(GetMousePosition(), camera);
    // Check collision between ray and model
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

    enum unicornParts {
        body = 0,
        mane,
        tail,
        horn
    } unicornParts;

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

    bool colorPaneActive;
    auto unicornBodyColor = PINK;
    auto unicornHornColor = GREEN;
    auto unicornManeColor = YELLOW;
    auto unicornTailColor = BLUE;

    bool exitWindow = false;

    int selectedPart = -1;

    auto clicked = false;

    Vector3 cubePosition = {0.0f, 1.0f, 0.0f};
    Vector3 cubeSize = {2.0f, 2.0f, 2.0f};
    Ray ray = {0};
    RayCollision collision = {false};

    int collisionModelIndex = 0;

    auto texture = LoadRenderTexture(screenWidth, screenHeight);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!IsKeyPressed(KEY_ESCAPE)){
        if (IsKeyPressed(KEY_ONE)) colorPaneActive = !colorPaneActive;
//        if (IsKeyPressed(KEY_TWO)) selectedPart = unicornParts.;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if (!colorPaneActive){
                collisionModelIndex = -1;
                for (int n = 0; n < models.size(); n++) {
                    collision = rayHit(camera, ray, models.at(n));
                    if (collision.hit) {
                        colorPaneActive = true;
                        collisionModelIndex = n;
                    }
                }
            }
        }

        BeginDrawing();

        ClearBackground(WHITE);

//        BeginTextureMode(texture);
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

//        if (collision.hit){
//            DrawCubeWires(unicornBodyPosition, cubeSize.x, cubeSize.y, cubeSize.z, MAROON);
//            DrawCubeWires(unicornBodyPosition, cubeSize.x + 0.2f, cubeSize.y + 0.2f, cubeSize.z + 0.2f, GREEN);
//            DrawCubeWires(unicornBodyPosition, cubeSize.x + 0.3f, cubeSize.y + 0.3f, cubeSize.z + 0.3f, GREEN);
//            DrawCubeWires(unicornBodyPosition, cubeSize.x + 0.4f, cubeSize.y + 0.4f, cubeSize.z + 0.4f, GREEN);
//        }

        EndMode3D();
//        EndTextureMode();
//        DrawTexture(texture.texture, screenWidth, screenHeight, BLACK);

        if (colorPaneActive){
            if (collisionModelIndex == 0 && !exitWindow) {
                exitWindow = GuiWindowBox((Rectangle) {40, 40, 240, 160}, "Body");
                unicornBodyColor = GuiColorPicker((Rectangle) {70, 70, 120, 120}, nullptr, unicornBodyColor);
            } else if (collisionModelIndex == 1 && !exitWindow) {
                exitWindow = GuiWindowBox((Rectangle) {40, 40, 240, 160}, "Hair");
                unicornHornColor = GuiColorPicker((Rectangle) {170, 110, 120, 120}, nullptr, unicornHornColor);
            } else if (collisionModelIndex == 2 && !exitWindow) {
                exitWindow = GuiWindowBox((Rectangle) {40, 40, 240, 160}, "Mane");
                unicornManeColor = GuiColorPicker((Rectangle) {170, 110, 120, 120}, nullptr, unicornManeColor);
            } else if (collisionModelIndex == 3 && !exitWindow) {
                exitWindow = GuiWindowBox((Rectangle) {40, 40, 240, 160}, "Tail");
                unicornTailColor = GuiColorPicker((Rectangle) {170, 110, 120, 120}, nullptr, unicornTailColor);
            } else {
                exitWindow = false;
            }
        }


//        GuiSetStyle(BUTTON, BORDER_WIDTH, 2);
//        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
//
//        if (GuiButton((Rectangle) {0, 1, 50, 20}, "Body")){
//            selectedPart = 0;
//            colorPaneActive = !colorPaneActive;
//        }
//        if (GuiButton((Rectangle) {0, 62, 50, 20}, "Horn")){
//            selectedPart = 1;
//            colorPaneActive = !colorPaneActive;
//        }
//        if (GuiButton((Rectangle) {0, 122, 50, 20}, "Tail")){
//            selectedPart = 2;
//            colorPaneActive = !colorPaneActive;
//        }
//        if (GuiButton((Rectangle) {0, 186, 50, 20}, "Mane")){
//            selectedPart = 3;
//            colorPaneActive = !colorPaneActive;
//        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}