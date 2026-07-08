#include "../../include/Systems/PlayerSystem.h"
#include "../../include/Systems/CameraSystem.h"
#include "../../include/Data/EntityData/ActionData.h"
#include "../../include/Data/WorldData/MapData.h"
#include "../../include/Systems/InteractionSystem.h"
#include "../../include/Systems/BuildingMenuSystem.h"
#include "../../cmake-build-debug/_deps/raylib-src/src/raylib.h"

// --- CLIENT-SIDE MANTIĞI ---
namespace Systems::PlayerSystem {

    // İleride ortak bir ayar (Config) veya MathUtils dosyasına alınabilir
    constexpr int TILE_SIZE = 32;

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

        // Piksel koordinatından Tile (Grid) koordinatına dönüşüm ve negatif alan düzeltmesi
        int targetGridX = worldMouse.x >= 0 ? static_cast<int>(worldMouse.x) / TILE_SIZE : (static_cast<int>(worldMouse.x) - TILE_SIZE + 1) / TILE_SIZE;
        int targetGridY = worldMouse.y >= 0 ? static_cast<int>(worldMouse.y) / TILE_SIZE : (static_cast<int>(worldMouse.y) - TILE_SIZE + 1) / TILE_SIZE;

        // CLANG-TIDY DÜZELTMESİ: {} ile ilk değer ataması (Zero-initialization) yapıldı
        Data::EntityData::PlayerAction requestedAction{};
        requestedAction.playerID = 1;
        requestedAction.type = Data::EntityData::ActionType::NONE;
        requestedAction.targetPos = {targetGridX, targetGridY};

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

            // Oyuncunun piksel pozisyonunu Tile pozisyonuna çevir (Negatif alan düzeltmeli)
            int playerGridX = player.position.x >= 0 ? player.position.x / TILE_SIZE : (player.position.x - TILE_SIZE + 1) / TILE_SIZE;
            int playerGridY = player.position.y >= 0 ? player.position.y / TILE_SIZE : (player.position.y - TILE_SIZE + 1) / TILE_SIZE;

            requestedAction.targetPos = {playerGridX, playerGridY};
        }

        if (requestedAction.type != Data::EntityData::ActionType::NONE) {
            // PARAMETRE DÜZELTMESİ: Fazladan olan 4. parametre (1.0f / 60.0f) silindi.
            Systems::InteractionSystem::ExecuteActionOnServer(player, map, requestedAction);
        } else {
            player.actionTimer = 0;
        }
    }
}