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

        // UI yapılana kadar test amaçlı: T tuşu ile inşa modunu aç/kapat
        static Data::WorldData::BuildingType selectedBuilding = Data::WorldData::BuildingType::NONE;
        if (IsKeyPressed(KEY_T)) {
            if (selectedBuilding == Data::WorldData::BuildingType::NONE)
                selectedBuilding = Data::WorldData::BuildingType::CONVEYOR;
            else
                selectedBuilding = Data::WorldData::BuildingType::NONE;
        }

        // SOL TIK BASILI TUTULDUĞUNDA (IsMouseButtonDown)
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            if (selectedBuilding == Data::WorldData::BuildingType::NONE) {
                // Seçili bina yoksa maden kaz
                requestedAction.type = Data::EntityData::ActionType::MINE_ORE;
            } else {
                // Seçili bina varsa inşa et
                requestedAction.type = Data::EntityData::ActionType::BUILD;
                requestedAction.buildType = selectedBuilding;
            }
        }
        // SAĞ TIK BASILI TUTULDUĞUNDA (Yıkım)
        else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            requestedAction.type = Data::EntityData::ActionType::DEMOLISH;
        }
        // E TUŞUNA BASILDIĞINDA (Tek tık - IsKeyPressed)
        else if (IsKeyPressed(KEY_E)) {
            requestedAction.type = Data::EntityData::ActionType::TRANSFER_BASE;
            requestedAction.gridX = (int)(player.position.x / map.tileSize);
            requestedAction.gridY = (int)(player.position.y / map.tileSize);
        }

        if (requestedAction.type != Data::EntityData::ActionType::NONE) {
            Systems::Simulation::InteractionSystem::ExecuteActionOnServer(player, map, requestedAction, dt);
        } else {
            player.actionTimer = 0.0f;
        }
    }
}