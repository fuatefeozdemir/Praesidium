#include "../../include/Systems/PlayerSystem.h"

#include "raylib.h"

#include "../../include/Data/EntityData/ActionData.h"
#include "../../include/Data/WorldData/MapData.h"
#include "../../include/Systems/BuildingMenuSystem.h"
#include "../../include/Systems/CameraSystem.h"
#include "../../include/Systems/InteractionSystem.h"
#include "../../include/Systems/MapSystem.h"

namespace {

    constexpr int TICKS_PER_SECOND = 60;

} // namespace

namespace Systems::PlayerSystem {

    void Update(Data::CoreData::GameContext* context) {

        // Shortcuts
        auto& player = context->player;
        auto& map = context->worldMap;
        auto* uiState = &context->buildingMenu;

        bool isMoving = false;
        const int moveStep = player.speed / TICKS_PER_SECOND;

        // Movement
        if (IsKeyDown(KEY_W)) {
            player.position.y -= moveStep;
            isMoving = true;
        }

        if (IsKeyDown(KEY_S)) {
            player.position.y += moveStep;
            isMoving = true;
        }

        if (IsKeyDown(KEY_A)) {
            player.position.x -= moveStep;
            isMoving = true;
        }

        if (IsKeyDown(KEY_D)) {
            player.position.x += moveStep;
            isMoving = true;
        }

        if (isMoving) {
            player.state = Data::EntityData::PlayerActionState::MOVING;
        } else if (player.state != Data::EntityData::PlayerActionState::BUILDING) {
            player.state = Data::EntityData::PlayerActionState::IDLE;
        }

        // Player actions
        const Vector2 worldMouse = Systems::CameraSystem::GetWorldMousePosition();

        Data::EntityData::PlayerAction requestedAction{};
        requestedAction.type = Data::EntityData::ActionType::NONE;

        // TODO: Replace with the actual player ID when multiplayer is implemented.
        requestedAction.playerID = 1;

        requestedAction.targetPos = Systems::MapSystem::PixelToTile(
            static_cast<int>(worldMouse.x),
            static_cast<int>(worldMouse.y)
        );

        const bool isBuildMode =
            uiState->selectedBuilding != Data::WorldData::BuildingType::NONE;

        // Consume UI input
        if (Systems::BuildingMenuSystem::Update(uiState)) {
            player.actionTimer = 0;
            return;
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {

            if (isBuildMode) {
                requestedAction.type = Data::EntityData::ActionType::BUILD;
                requestedAction.buildType = uiState->selectedBuilding;
            } else {
                requestedAction.type = Data::EntityData::ActionType::MINE_ORE;
            }

        } else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && !isBuildMode) {

            requestedAction.type = Data::EntityData::ActionType::DEMOLISH;

        } else if (IsKeyPressed(KEY_E)) {

            requestedAction.type = Data::EntityData::ActionType::TRANSFER_BASE;

            requestedAction.targetPos = Systems::MapSystem::PixelToTile(
                player.position.x,
                player.position.y
            );
        }

        if (requestedAction.type == Data::EntityData::ActionType::NONE) {
            player.actionTimer = 0;
            return;
        }

        Systems::InteractionSystem::ExecuteActionOnServer(
            player,
            map,
            requestedAction
        );
    }

} // namespace Systems::PlayerSystem