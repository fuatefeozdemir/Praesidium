#include "raylib.h"
#include "../include/Engine/AppState.h"
#include "../include/Engine/WindowManager.h"

int main() {
    // Pencereyi ve ekran ayarlarını dış sistemden başlat
    WindowManager::Initialize();

    AppState currentState = AppState::MAIN_MENU;

    // Ana Oyun Döngüsü
    while (!WindowShouldClose() && currentState != AppState::EXIT_REQUESTED) {

        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

        // --- Güncelleme Kontrolleri ---
        switch (currentState) {
            case AppState::MAIN_MENU:
                if (IsKeyPressed(KEY_ENTER)) currentState = AppState::ACTIVE_SIMULATION;
                if (IsKeyPressed(KEY_ESCAPE)) currentState = AppState::EXIT_REQUESTED;
                break;
            case AppState::ACTIVE_SIMULATION:
                if (IsKeyPressed(KEY_ESCAPE)) currentState = AppState::PAUSED;
                break;
            case AppState::PAUSED:
                if (IsKeyPressed(KEY_ESCAPE)) currentState = AppState::ACTIVE_SIMULATION;
                if (IsKeyPressed(KEY_Q)) currentState = AppState::MAIN_MENU;
                break;
            default:
                break;
        }

        // --- Çizim ---
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentState) {
            case AppState::MAIN_MENU:
                ClearBackground(RAYWHITE);
                DrawText("PRAESIDIUM - ANA MENU", 10, 10, 20, DARKBLUE);
                DrawText("Oyuna gecmek icin ENTER'a bas", 10, 40, 20, GRAY);
                DrawText("Cikmak icin ESC'ye bas", 10, 70, 20, GRAY);
                break;
            case AppState::ACTIVE_SIMULATION:
                ClearBackground(DARKGREEN);
                DrawText("SIMULASYON CALISIYOR", 10, 10, 20, RAYWHITE);
                DrawText("Duraklatmak icin ESC'ye bas", 10, 40, 20, LIGHTGRAY);
                break;
            case AppState::PAUSED:
                ClearBackground(DARKGRAY);
                DrawText("OYUN DURAKLATILDI", 10, 10, 20, RED);
                DrawText("Devam: ESC | Ana Menu: Q", 10, 40, 20, LIGHTGRAY);
                break;
            default:
                break;
        }

        EndDrawing();
    }

    WindowManager::Close();

    return 0;
}