#include "raylib.h"
#include "../include/Data/CoreData/GameContext.h"
#include "../include/Engine/Core/Window.h"
#include "../include/Engine/Core/Camera.h"
#include "../include/Interface/UI/CoreUI/MainMenuRenderer.h"
#include "../include/Systems/Simulation/PlayerSystem.h"
#include "../include/Systems/World/MapSystem.h"
#include "../include/Systems/World/BuildingSystem.h"
#include "../include/Interface/World/BuildingRenderer.h"
#include "../include/Interface/World/MapRenderer.h"
#include "../include/Interface/UI/CoreUI/BuildingMenuRenderer.h"

int main() {
    Engine::Core::Window::Initialize();
    Engine::Core::Camera::Initialize();
    Interface::UI::CoreUI::MainMenuSystem::Initialize();

    // 1. MERKEZİ OYUN BAĞLAMINI (CONTEXT) OLUŞTUR
    Data::CoreData::GameContext gameContext;

    // 2. CONTEXT İÇİNDEKİ VERİLERİ BAŞLAT
    gameContext.player = {
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

    Systems::World::MapSystem::Initialize(gameContext.worldMap, 100, 100, 64);

    // 3. ANA DÖNGÜ (Artık durum kontrolü gameContext üzerinden yapılıyor)
    while (!WindowShouldClose() && gameContext.currentState != AppState::EXIT_REQUESTED) {

        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();

        // --- GÜNCELLEME (LOGIC) ---
        switch (gameContext.currentState) {
            case AppState::MAIN_MENU:
                Interface::UI::CoreUI::MainMenuSystem::Update(gameContext.currentState);
                break;
            case AppState::ACTIVE_SIMULATION:
                // SİSTEME SADECE CONTEXT'İN ADRESİNİ GÖNDERİYORUZ
                Systems::Simulation::PlayerSystem::Update(&gameContext);
                Engine::Core::Camera::Update(gameContext.player.position);

                if (IsKeyPressed(KEY_ESCAPE)) gameContext.currentState = AppState::PAUSED;
                break;
            case AppState::PAUSED:
                if (IsKeyPressed(KEY_ESCAPE)) gameContext.currentState = AppState::ACTIVE_SIMULATION;
                if (IsKeyPressed(KEY_Q)) gameContext.currentState = AppState::MAIN_MENU;
                break;
            default: break;
        }

        // --- ÇİZİM (RENDER) ---
        BeginDrawing();
        ClearBackground(BLACK);

        switch (gameContext.currentState) {
            case AppState::MAIN_MENU:
                Interface::UI::CoreUI::MainMenuSystem::Draw();
                break;
            case AppState::ACTIVE_SIMULATION: {
                ClearBackground(Color{ 20, 20, 20, 255 });

                Engine::Core::Camera::BeginWorldDrawing();

                // 1. Haritayı Çiz
                Interface::World::MapRenderer::Draw(&gameContext);

                // 2. Binaları Çiz
                Interface::World::BuildingRenderer::Draw(&gameContext);

                // 3. Karakteri Çiz
                DrawCircleV(gameContext.player.position, gameContext.player.collisionRadius, BLUE);

                // 4. Farenin dünyadaki konumunu çiz
                Vector2 worldMouse = Engine::Core::Camera::GetWorldMousePosition();
                DrawCircleV(worldMouse, 5.0f, RED);

                Engine::Core::Camera::EndWorldDrawing();

                // 5. Arayüzü Çiz
                Interface::UI::CoreUI::BuildingMenuRenderer::Draw(&gameContext);

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

    Interface::UI::CoreUI::MainMenuSystem::Close();
    Engine::Core::Window::Close();
    return 0;
}