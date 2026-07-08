#include "../../include/Systems/InteractionSystem.h"
#include "../../include/Systems/MapSystem.h"
#include "../../include/Systems/BuildingSystem.h"

#include <cmath>
#include <iostream>

namespace Systems::InteractionSystem {

    bool IsWithinInteractionRange(Data::CoreData::Vector2Int playerTile,
                                  Data::CoreData::Vector2Int targetTile,
                                  int maxRange) {
        return (std::abs(playerTile.x - targetTile.x) <= maxRange) &&
               (std::abs(playerTile.y - targetTile.y) <= maxRange);
    }

    void ExecuteActionOnServer(Data::EntityData::Player& player,
                               Data::WorldData::Map& map,
                               const Data::EntityData::PlayerAction& action) {

        Data::CoreData::Vector2Int targetTile = action.targetPos;
        Data::CoreData::Vector2Int chunkPos = Systems::MapSystem::WorldToChunk(targetTile);

        if (!Systems::MapSystem::IsInsideWorld(map, chunkPos))
            return;

        Data::CoreData::Vector2Int playerTile =
            Systems::MapSystem::PixelToTile(player.position.x, player.position.y);

        if (player.lastTargetX != targetTile.x || player.lastTargetY != targetTile.y) {
            player.actionTimer = 0;
            player.lastTargetX = targetTile.x;
            player.lastTargetY = targetTile.y;
        }

        Data::WorldData::Tile* tile = Systems::MapSystem::GetOrCreateTile(map, targetTile);
        if (!tile)
            return;

        switch (action.type) {

            case Data::EntityData::ActionType::MINE_ORE: {

                if (!IsWithinInteractionRange(playerTile, targetTile, 2))
                    return;

                auto targetOre = tile->ore;

                if (targetOre != Data::WorldData::ItemType::NONE) {

                    player.actionTimer++;

                    if (player.actionTimer >= 60) {

                        int itemID = static_cast<int>(targetOre);
                        bool found = false;

                        for (auto& slot : player.inventory) {
                            if (slot.itemID == itemID) {
                                slot.count++;
                                found = true;
                                break;
                            }
                        }

                        if (!found && player.inventory.size() < player.maxSlots) {
                            player.inventory.push_back({itemID, 1});
                        }

                        std::cout << "Maden kazildi." << std::endl;
                        player.actionTimer = 0;
                    }
                }

                break;
            }

            case Data::EntityData::ActionType::BUILD: {

                if (!IsWithinInteractionRange(playerTile, targetTile, 3))
                    return;

                if (tile->buildingID != -1) {
                    std::cout << "Burada zaten bir bina var." << std::endl;
                    break;
                }

                auto id = Systems::BuildingSystem::CreateBuilding(
                    map,
                    action.buildType,
                    targetTile,
                    Data::WorldData::Direction::NORTH
                );

                if (id != -1) {
                    std::cout << "Bina olusturuldu. ID: " << id << std::endl;
                } else {
                    std::cout << "Bina yerlestirilemedi." << std::endl;
                }

                break;
            }

            case Data::EntityData::ActionType::DEMOLISH: {

                if (!IsWithinInteractionRange(playerTile, targetTile, 3))
                    return;

                if (tile->buildingID == -1)
                    return;

                Systems::BuildingSystem::DestroyBuilding(map, tile->buildingID);

                std::cout << "Yapi silindi." << std::endl;

                break;
            }

            case Data::EntityData::ActionType::TRANSFER_BASE: {

                std::cout << "TRANSFER_BASE henuz uygulanmadi." << std::endl;

                break;
            }

            default:
                break;
        }
    }

}