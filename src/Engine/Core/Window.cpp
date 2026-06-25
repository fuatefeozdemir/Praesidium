#include "../../../include/Engine/Core/Window.h"
#include "raylib.h"

namespace Engine::Core::Window {
    void Initialize() {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(STARTUP_WIDTH, STARTUP_HEIGHT, WINDOW_TITLE);
        SetTargetFPS(TARGET_FPS);
        SetExitKey(0);
    }

    void Close() {
        CloseWindow();
    }
}