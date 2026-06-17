#include "../../../include/Engine/Core/WindowManager.h"
#include "../../../include/Data/CoreData/Configuration.h"
#include "raylib.h"

namespace WindowManager {
    void Initialize() {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(Config::DEFAULT_WINDOW_WIDTH, Config::DEFAULT_WINDOW_HEIGHT, Config::WINDOW_TITLE);
        SetTargetFPS(Config::TARGET_FPS);
        SetExitKey(0);
    }

    void Close() {
        CloseWindow();
    }
}