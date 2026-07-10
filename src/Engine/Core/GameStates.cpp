#include "../../../include/Engine/Core/GameStates.h"

#include "../../../include/Interface/UI/BuildingMenuRenderer.h"
#include "../../../include/Interface/UI/MainMenuRenderer.h"
#include "../../../include/Interface/World/MapRenderer.h"
#include "../../../include/Interface/World/PlayerRenderer.h"
#include "../../../include/Systems/CameraSystem.h"
#include "../../../include/Systems/PlayerSystem.h"
#include "../../../include/Systems/ChunkSystem.h"

#include "raylib.h"

namespace {

    Vector2 ToVector2(const Data::CoreData::Vector2Int& position) {
        return {
            static_cast<float>(position.x),
            static_cast<float>(position.y)
        };
    }

} // namespace

namespace Engine::Core {

    // Main menu state

    void MainMenuState::Update(Data::CoreData::GameContext* context) {
        Interface::UI::MainMenuRenderer::Update(context->currentState);
    }

    void MainMenuState::Draw(Data::CoreData::GameContext* context) {
        ClearBackground(BLACK);
        Interface::UI::MainMenuRenderer::Draw();
    }

    // Active simulation state

    void ActiveSimulationState::Update(Data::CoreData::GameContext* context) {

        Systems::PlayerSystem::Update(context);

        Systems::CameraSystem::SetTarget(
            ToVector2(context->player.position)
        );

        Systems::CameraSystem::Update();

        Systems::ChunkSystem::Update(context);

        if (IsKeyPressed(KEY_ESCAPE)) {
            context->currentState =
                Data::CoreData::AppState::PAUSED;
        }
    }

    void ActiveSimulationState::Draw(Data::CoreData::GameContext* context) {
        ClearBackground(Color{20, 20, 20, 255});

        Systems::CameraSystem::BeginWorldDrawing();

        Interface::World::MapRenderer::Draw(context->worldMap);
        Interface::World::PlayerRenderer::Draw(context->player);

        Systems::CameraSystem::EndWorldDrawing();

        Interface::UI::BuildingMenuRenderer::Draw(context);
    }

    // Pause menu state

    void PausedState::Update(Data::CoreData::GameContext* context) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            context->currentState = Data::CoreData::AppState::ACTIVE_SIMULATION;
        }

        if (IsKeyPressed(KEY_Q)) {
            context->currentState = Data::CoreData::AppState::MAIN_MENU;
        }
    }

    void PausedState::Draw(Data::CoreData::GameContext* context) {
        ClearBackground(DARKGRAY);

        // TODO: Replace with dedicated pause menu UI.
        DrawText("OYUN DURAKLATILDI", 10, 10, 20, RED);
        DrawText("Devam: ESC | Ana Menu: Q", 10, 40, 20, LIGHTGRAY);
    }

} // namespace Engine::Core