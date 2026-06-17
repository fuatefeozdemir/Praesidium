#include "../../include/Engine/WindowManager.h"
#include "../../include/Engine/Configuration.h"
#include "raylib.h"

namespace WindowManager {
    void Initialize() {
        // Pencereyi başlat ve monitör boyutuna göre ayarla
        InitWindow(Config::DEFAULT_WINDOW_WIDTH, Config::DEFAULT_WINDOW_HEIGHT, Config::WINDOW_TITLE);

        int currentMonitor = GetCurrentMonitor();
        int monitorWidth = GetMonitorWidth(currentMonitor);
        int monitorHeight = GetMonitorHeight(currentMonitor);

        SetWindowSize(monitorWidth, monitorHeight);
        ToggleFullscreen();
        SetTargetFPS(Config::TARGET_FPS);

        SetExitKey(0); // Raylib'in varsayılan ESC ile kapatma özelliğini devre dışı bırak
    }

    void Close() {
        CloseWindow();
    }
}