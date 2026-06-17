#include "raylib.h"
#include <vector>
#include "../include/Engine/AppState.h"
#include "../include/Engine/WindowManager.h"
#include "../include/Interface/UIData.h"
#include "../include/Interface/MenuController.h"
#include "../include/Interface/RenderLayer.h"

int main() {
    WindowManager::Initialize();

    AppState currentState = AppState::MAIN_MENU;

    // --- ARAYÜZ VERİLERİNİN OLUŞTURULMASI ---
    // Ekran genişliği 1280. Ortalamak için: (1280 / 2) - (200 / 2) = 540
    std::vector<UI::Button> mainMenuButtons = {
        {{540, 300, 200, 50}, "OYUNA BASLA", UI::ButtonState::NORMAL},
        {{540, 380, 200, 50}, "CIKIS", UI::ButtonState::NORMAL}
    };

    while (!WindowShouldClose() && currentState != AppState::EXIT_REQUESTED) {

        // --- Global Girdiler ---
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

        // --- Güncelleme Kontrolleri ---
        switch (currentState) {
            case AppState::MAIN_MENU: {
                int currentWidth = GetScreenWidth();
                int currentHeight = GetScreenHeight();

                mainMenuButtons[0].bounds.x = (currentWidth / 2.0f) - (mainMenuButtons[0].bounds.width / 2.0f);
                mainMenuButtons[0].bounds.y = (currentHeight / 2.0f) - 60.0f;

                mainMenuButtons[1].bounds.x = (currentWidth / 2.0f) - (mainMenuButtons[1].bounds.width / 2.0f);
                mainMenuButtons[1].bounds.y = (currentHeight / 2.0f) + 20.0f;

                Interface::UpdateMainMenu(mainMenuButtons, currentState);
                break;
            }
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

        // --- Render ---
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentState) {
            case AppState::MAIN_MENU:
                Interface::DrawMainMenu(mainMenuButtons);
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