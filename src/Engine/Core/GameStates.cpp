#include "../../../include/Engine/Core/GameStates.h"
#include "raylib.h"
#include "../../../include/Systems/PlayerSystem.h"
#include "../../../include/Interface/UI/MainMenuRenderer.h"
#include "../../../include/Interface/World/MapRenderer.h"
#include "../../../include/Interface/World/BuildingRenderer.h"
#include "../../../include/Interface/UI/BuildingMenuRenderer.h"
#include "../../../include/Engine/Core/Camera.h"

namespace Engine::Core {

    // --- MAIN MENU ---
    void MainMenuState::Update(Data::CoreData::GameContext* context) {
        Interface::UI::MainMenuRenderer::Update(context->currentState);
    }

    void MainMenuState::Draw(Data::CoreData::GameContext* context) {
        ClearBackground(BLACK);
        Interface::UI::MainMenuRenderer::Draw();
    }

    // --- ACTIVE SIMULATION ---
    void ActiveSimulationState::Update(Data::CoreData::GameContext* context) {
        Systems::PlayerSystem::Update(context);
        Engine::Core::Camera::Update(context->player.position);
        
        if (IsKeyPressed(KEY_ESCAPE)) context->currentState = AppState::PAUSED;
    }

    void ActiveSimulationState::Draw(Data::CoreData::GameContext* context) {
        ClearBackground(Color{ 20, 20, 20, 255 });

        Engine::Core::Camera::BeginWorldDrawing();
        Interface::World::MapRenderer::Draw(context);
        Interface::World::BuildingRenderer::Draw(context);
        DrawCircleV(context->player.position, context->player.collisionRadius, BLUE);
        DrawCircleV(Engine::Core::Camera::GetWorldMousePosition(), 5.0f, RED);
        Engine::Core::Camera::EndWorldDrawing();

        Interface::UI::BuildingMenuRenderer::Draw(context);
    }

    // --- PAUSED ---
    void PausedState::Update(Data::CoreData::GameContext* context) {
        if (IsKeyPressed(KEY_ESCAPE)) context->currentState = AppState::ACTIVE_SIMULATION;
        if (IsKeyPressed(KEY_Q)) context->currentState = AppState::MAIN_MENU;
    }

    void PausedState::Draw(Data::CoreData::GameContext* context) {
        ClearBackground(DARKGRAY);
        DrawText("OYUN DURAKLATILDI", 10, 10, 20, RED);
        DrawText("Devam: ESC | Ana Menu: Q", 10, 40, 20, LIGHTGRAY);
    }
}