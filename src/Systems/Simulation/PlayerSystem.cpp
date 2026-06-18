#include "../../../include/Systems/Simulation/PlayerSystem.h"
#include "../../../include/Engine/Core/CameraManager.h"
#include "../../../include/Data/EntityData/ActionData.h"
#include "../../../include/Data/WorldData/MapData.h"
#include "../../../include/Systems/Simulation/InteractionSystem.h"
#include "raylib.h"

// --- CLIENT-SIDE MANTIĞI ---
namespace Systems::Simulation::PlayerSystem {
    void Update(Data::EntityData::Player& player, Data::WorldData::Map& map) {
        float dt = GetFrameTime();
        bool isMoving = false;

        // 1. Hareket Kontrolü
        if (IsKeyDown(KEY_W)) { player.position.y -= player.speed * dt; isMoving = true; }
        if (IsKeyDown(KEY_S)) { player.position.y += player.speed * dt; isMoving = true; }
        if (IsKeyDown(KEY_A)) { player.position.x -= player.speed * dt; isMoving = true; }
        if (IsKeyDown(KEY_D)) { player.position.x += player.speed * dt; isMoving = true; }

        if (isMoving) {
            player.state = Data::EntityData::PlayerActionState::MOVING;
        } else if (player.state != Data::EntityData::PlayerActionState::BUILDING) {
            player.state = Data::EntityData::PlayerActionState::IDLE;
        }

        // 2. Etkileşim İstekleri (Client)
        Vector2 worldMouse = Engine::Core::CameraManager::GetWorldMousePosition();
        int targetGridX = (int)(worldMouse.x / map.tileSize);
        int targetGridY = (int)(worldMouse.y / map.tileSize);

        Data::EntityData::PlayerAction requestedAction;
        requestedAction.playerID = 1;
        requestedAction.type = Data::EntityData::ActionType::NONE;
        requestedAction.gridX = targetGridX;
        requestedAction.gridY = targetGridY;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            requestedAction.type = Data::EntityData::ActionType::BUILD;
            requestedAction.buildType = Data::WorldData::BuildingType::CONVEYOR;
        } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            requestedAction.type = Data::EntityData::ActionType::MINE_ORE;
        } else if (IsKeyPressed(KEY_E)) {
            requestedAction.type = Data::EntityData::ActionType::TRANSFER_BASE;
            requestedAction.gridX = (int)(player.position.x / map.tileSize);
            requestedAction.gridY = (int)(player.position.y / map.tileSize);
        }

        // İstek varsa sunucu fonksiyonuna gönder
        if (requestedAction.type != Data::EntityData::ActionType::NONE) {
            Systems::Simulation::InteractionSystem::ExecuteActionOnServer(player, map, requestedAction);
        }
    }
}