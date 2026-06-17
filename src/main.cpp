#include "raylib.h"
#include "../include/Data/CoreData/AppState.h"
#include "../include/Engine/Core/WindowManager.h"
#include "../include/Engine/Core/CameraManager.h"
#include "../include/Interface/UI/CoreUI/MainMenuSystem.h"
#include "../include/Data/EntityData/PlayerData.h"
#include "../include/Systems/Simulation/PlayerSystem.h"
#include "../include/Systems/Simulation/MapSystem.h"
#include "../include/Data/WorldData/MapData.h"
#include "../include/Interface/World/MapRenderer.h"

int main() {
    WindowManager::Initialize();
    Engine::Core::CameraManager::Initialize();
    Interface::UI::CoreUI::MainMenuSystem::Initialize();

    AppState currentState = AppState::MAIN_MENU;

    // Karakterin başlangıç verisini (Data) oluştur
    Data::EntityData::Player player = {
        "Oyuncu",                         // name
        {0.0f, 0.0f},                     // position (Dünyanın tam merkezi)
        300.0f,                           // speed
        15.0f,                            // collisionRadius
        Data::EntityData::PlayerActionState::IDLE, // state
        100.0f,                           // health
        100.0f,                           // maxHealth
        20,                               // maxSlots
        {}                                // inventory (boş)
    };

    Data::WorldData::Map worldMap;
    Systems::Simulation::MapSystem::Initialize(worldMap, 100, 100, 64);

    while (!WindowShouldClose() && currentState != AppState::EXIT_REQUESTED) {

        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();

        // --- GÜNCELLEME (LOGIC) ---
        switch (currentState) {
            case AppState::MAIN_MENU:
                Interface::UI::CoreUI::MainMenuSystem::Update(currentState);
                break;
            case AppState::ACTIVE_SIMULATION:

                Systems::Simulation::PlayerSystem::Update(player);
                Engine::Core::CameraManager::Update(player.position);

                if (IsKeyPressed(KEY_ESCAPE)) currentState = AppState::PAUSED;
                break;
            case AppState::PAUSED:
                if (IsKeyPressed(KEY_ESCAPE)) currentState = AppState::ACTIVE_SIMULATION;
                if (IsKeyPressed(KEY_Q)) currentState = AppState::MAIN_MENU;
                break;
            default: break;
        }

        // --- ÇİZİM (RENDER) ---
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentState) {
            case AppState::MAIN_MENU:
                Interface::UI::CoreUI::MainMenuSystem::Draw();
            break;
            case AppState::ACTIVE_SIMULATION: {
                ClearBackground(Color{ 20, 20, 20, 255 });

                Engine::Core::CameraManager::BeginWorldDrawing();

                // 1. Haritayı Çiz
                Interface::World::MapRenderer::Draw(worldMap);

                // 2. Karakteri Çiz
                DrawCircleV(player.position, player.collisionRadius, BLUE);

                // 3. Farenin dünyadaki konumunu çiz
                Vector2 worldMouse = Engine::Core::CameraManager::GetWorldMousePosition();
                DrawCircleV(worldMouse, 5.0f, RED);

                Engine::Core::CameraManager::EndWorldDrawing();

                // Ekrana sabit UI çizimleri
                DrawText("SIMULASYON CALISIYOR", 10, 10, 20, RAYWHITE);
                DrawText(TextFormat("Durum: %s", player.state == Data::EntityData::PlayerActionState::MOVING ? "HAREKET EDIYOR" : "BEKLIYOR"), 10, 40, 20, LIGHTGRAY);
                break;
            }
            case AppState::PAUSED:
                ClearBackground(DARKGRAY);
            DrawText("OYUN DURAKLATILDI", 10, 10, 20, RED);
            DrawText("Devam: ESC | Ana Menu: Q", 10, 40, 20, LIGHTGRAY);
            break;
            default: break;
        }

        EndDrawing();
    }

    WindowManager::Close();
    return 0;
}
