#include "../../../include/Systems/Simulation/InteractionSystem.h"
#include <cmath>
#include <iostream>

#include "../../../include/Data/EntityData/ActionData.h"
#include "../../../include/Data/EntityData/PlayerData.h"
#include "../../../include/Data/WorldData/MapData.h"

namespace Systems::Simulation::InteractionSystem {
    
    // Sadece bu dosyada kullanılacak yardımcı fonksiyon
    bool IsWithinInteractionRange(int playerX, int playerY, int targetX, int targetY, int maxRange) {
        return (std::abs(playerX - targetX) <= maxRange) && (std::abs(playerY - targetY) <= maxRange);
    }

    void ExecuteActionOnServer(Data::EntityData::Player& player, Data::WorldData::Map& map, const Data::EntityData::PlayerAction& action) {
        if (action.gridX < 0 || action.gridX >= map.width || action.gridY < 0 || action.gridY >= map.height) return;

        int playerGridX = (int)(player.position.x / map.tileSize);
        int playerGridY = (int)(player.position.y / map.tileSize);
        int targetIndex = action.gridY * map.width + action.gridX;

        switch (action.type) {
            case Data::EntityData::ActionType::MINE_ORE: {
                if (!IsWithinInteractionRange(playerGridX, playerGridY, action.gridX, action.gridY, 2)) return;
                
                auto targetOre = map.tiles[targetIndex].ore;
                if (targetOre != Data::WorldData::OreType::NONE) {
                    std::cout << "Maden kazildi! Tur: " << (int)targetOre << std::endl;
                }
                break;
            }
            case Data::EntityData::ActionType::BUILD: {
                if (!IsWithinInteractionRange(playerGridX, playerGridY, action.gridX, action.gridY, 3)) return;
                if (map.tiles[targetIndex].buildingID != -1) return;

                std::cout << "Bina insa edildi! Koordinat: " << action.gridX << ", " << action.gridY << std::endl;
                break;
            }
            case Data::EntityData::ActionType::TRANSFER_BASE: {
                int bID = map.tiles[targetIndex].buildingID;
                if (bID != -1) {
                    std::cout << "Esyalar usse aktarildi!" << std::endl;
                }
                break;
            }
            default: break;
        }
    }
}
