#include "raylib.h"
#include "../include/Data/CoreData/AppState.h"
#include "../include/Engine/Core/WindowManager.h"
#include "../include/Interface/UI/CoreUI/MainMenuSystem.h"

int main() {
    WindowManager::Initialize();
    Interface::UI::CoreUI::MainMenuSystem::Initialize();

    AppState currentState = AppState::MAIN_MENU;

    while (!WindowShouldClose() && currentState != AppState::EXIT_REQUESTED) {

        // --- GLOBAL GİRDİLER ---
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

        // --- GÜNCELLEME KONTROLLERİ ---
        switch (currentState) {
            case AppState::MAIN_MENU:
                Interface::UI::CoreUI::MainMenuSystem::Update(currentState);
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

        // --- ÇİZİM İŞLEMLERİ ---
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentState) {
            case AppState::MAIN_MENU:
                Interface::UI::CoreUI::MainMenuSystem::Draw();
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