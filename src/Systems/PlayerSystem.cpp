#include "../../include/Systems/PlayerSystem.h"
#include "../../include/Engine/Core/Camera.h"
#include "../../include/Data/EntityData/ActionData.h"
#include "../../include/Data/WorldData/MapData.h"
#include "../../include/Systems/InteractionSystem.h"
#include "../../include/Systems/BuildingMenuSystem.h"
#include "../../cmake-build-debug/_deps/raylib-src/src/raylib.h"

// --- CLIENT-SIDE MANTIĞI ---
namespace Systems::PlayerSystem {
    void Update(Data::CoreData::GameContext* context) {
        // context içindeki verileri referansa alıyoruz
        auto& player = context->player;
        auto& map = context->worldMap;
        auto* uiState = &context->buildingMenu;

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
        Vector2 worldMouse = Engine::Core::Camera::GetWorldMousePosition();
        int targetGridX = (int)(worldMouse.x / map.tileSize);
        int targetGridY = (int)(worldMouse.y / map.tileSize);

        Data::EntityData::PlayerAction requestedAction;
        requestedAction.playerID = 1;
        requestedAction.type = Data::EntityData::ActionType::NONE;
        requestedAction.gridX = targetGridX;
        requestedAction.gridY = targetGridY;

        // EĞER FARE MENÜ ÜZERİNDEYSE VEYA MENÜ İKONUNA TIKLANDIYSA (Tıklama Tüketimi)
        if (Systems::BuildingMenuSystem::Update(uiState)) {
            player.actionTimer = 0.0f; // Varsa mevcut eylemi kes
            return; // Harita etkileşimine izin verme
        }

        // SOL TIK BASILI TUTULDUĞUNDA (IsMouseButtonDown)
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            if (uiState->selectedBuilding == Data::WorldData::BuildingType::NONE) {
                // Seçili bina yoksa maden kaz
                requestedAction.type = Data::EntityData::ActionType::MINE_ORE;
            } else {
                // Seçili bina varsa inşa et
                requestedAction.type = Data::EntityData::ActionType::BUILD;
                requestedAction.buildType = uiState->selectedBuilding;
            }
        }
        // SAĞ TIK BASILI TUTULDUĞUNDA (Yıkım - Sadece inşa modu kapalıyken)
        else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && uiState->selectedBuilding == Data::WorldData::BuildingType::NONE) {
            requestedAction.type = Data::EntityData::ActionType::DEMOLISH;
        }
        // E TUŞUNA BASILDIĞINDA (Tek tık - IsKeyPressed)
        else if (IsKeyPressed(KEY_E)) {
            requestedAction.type = Data::EntityData::ActionType::TRANSFER_BASE;
            requestedAction.gridX = (int)(player.position.x / map.tileSize);
            requestedAction.gridY = (int)(player.position.y / map.tileSize);
        }

        if (requestedAction.type != Data::EntityData::ActionType::NONE) {
            Systems::InteractionSystem::ExecuteActionOnServer(player, map, requestedAction, dt);
        } else {
            player.actionTimer = 0.0f;
        }
    }
}