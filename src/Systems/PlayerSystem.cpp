#include "../../include/Systems/PlayerSystem.h"
#include "../../include/Systems/CameraSystem.h"
#include "../../include/Systems/MapSystem.h"
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

        bool isMoving = false;
        int moveStep = player.speed / 60;

        // 1. Hareket Kontrolü
        if (IsKeyDown(KEY_W)) { player.position.y -= moveStep; isMoving = true; }
        if (IsKeyDown(KEY_S)) { player.position.y += moveStep; isMoving = true; }
        if (IsKeyDown(KEY_A)) { player.position.x -= moveStep; isMoving = true; }
        if (IsKeyDown(KEY_D)) { player.position.x += moveStep; isMoving = true; }

        if (isMoving) {
            player.state = Data::EntityData::PlayerActionState::MOVING;
        } else if (player.state != Data::EntityData::PlayerActionState::BUILDING) {
            player.state = Data::EntityData::PlayerActionState::IDLE;
        }

        // 2. Etkileşim İstekleri (Client)
        Vector2 worldMouse = Systems::CameraSystem::GetWorldMousePosition();

        // CLANG-TIDY DÜZELTMESİ: {} ile ilk değer ataması (Zero-initialization) yapıldı
        Data::EntityData::PlayerAction requestedAction{};
        requestedAction.playerID = 1;
        requestedAction.type = Data::EntityData::ActionType::NONE;

        // Yeni API kullanımı ile matematiği MapSystem'e devrettik
        requestedAction.targetPos = Systems::MapSystem::PixelToTile(static_cast<int>(worldMouse.x), static_cast<int>(worldMouse.y));

        // EĞER FARE MENÜ ÜZERİNDEYSE VEYA MENÜ İKONUNA TIKLANDIYSA (Tıklama Tüketimi)
        if (Systems::BuildingMenuSystem::Update(uiState)) {
            player.actionTimer = 0; // Varsa mevcut eylemi kes
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

            // Oyuncunun piksel pozisyonunu Tile pozisyonuna çevir
            requestedAction.targetPos = Systems::MapSystem::PixelToTile(player.position.x, player.position.y);
        }

        if (requestedAction.type != Data::EntityData::ActionType::NONE) {
            Systems::InteractionSystem::ExecuteActionOnServer(player, map, requestedAction);
        } else {
            player.actionTimer = 0;
        }
    }
}