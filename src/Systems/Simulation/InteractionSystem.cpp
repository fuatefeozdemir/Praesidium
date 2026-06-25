#include "../../../include/Systems/Simulation/InteractionSystem.h"
#include <cmath>
#include <iostream>

#include "../../../include/Systems/World/BuildingSystem.h"

namespace Systems::Simulation::InteractionSystem {

    bool IsWithinInteractionRange(int playerX, int playerY, int targetX, int targetY, int maxRange) {
        return (std::abs(playerX - targetX) <= maxRange) && (std::abs(playerY - targetY) <= maxRange);
    }

    void ExecuteActionOnServer(Data::EntityData::Player& player, Data::WorldData::Map& map, const Data::EntityData::PlayerAction& action, float dt) {
        if (action.gridX < 0 || action.gridX >= map.width || action.gridY < 0 || action.gridY >= map.height) return;

        int playerGridX = (int)(player.position.x / map.tileSize);
        int playerGridY = (int)(player.position.y / map.tileSize);
        int targetIndex = action.gridY * map.width + action.gridX;

        // Oyuncu hedefini değiştirirse sayacı sıfırla
        if (player.lastTargetX != action.gridX || player.lastTargetY != action.gridY) {
            player.actionTimer = 0.0f;
            player.lastTargetX = action.gridX;
            player.lastTargetY = action.gridY;
        }

        switch (action.type) {
            case Data::EntityData::ActionType::MINE_ORE: {
                if (!IsWithinInteractionRange(playerGridX, playerGridY, action.gridX, action.gridY, 2)) return;

                auto targetOre = map.tiles[targetIndex].ore;
                if (targetOre != Data::WorldData::OreType::NONE) {
                    player.actionTimer += dt;
                    if (player.actionTimer >= 1.0f) {

                        Data::WorldData::ItemType minedItem = Data::WorldData::ItemType::NONE;
                        if (targetOre == Data::WorldData::OreType::IRON) minedItem = Data::WorldData::ItemType::IRON_ORE;
                        else if (targetOre == Data::WorldData::OreType::COPPER) minedItem = Data::WorldData::ItemType::COPPER_ORE;
                        else if (targetOre == Data::WorldData::OreType::COAL) minedItem = Data::WorldData::ItemType::COAL;

                        if (minedItem != Data::WorldData::ItemType::NONE) {
                            int itemID = static_cast<int>(minedItem);
                            bool itemFound = false;

                            // Envanterde eşyayı ara
                            for (auto& slot : player.inventory) {
                                if (slot.itemID == itemID) {
                                    slot.count++;
                                    std::cout << "Maden kazildi! Envanterdeki sayisi: " << slot.count << std::endl;
                                    itemFound = true;
                                    break;
                                }
                            }

                            // Eşya envanterde yoksa yeni slot aç
                            if (!itemFound) {
                                if (player.inventory.size() < player.maxSlots) {
                                    player.inventory.push_back({itemID, 1});
                                    std::cout << "Maden kazildi! Envanterdeki sayisi: 1" << std::endl;
                                } else {
                                    std::cout << "Envanter dolu! Maden alinamadi." << std::endl;
                                }
                            }
                        }
                        player.actionTimer = 0.0f;
                    }
                }
                break;
            }
            case Data::EntityData::ActionType::BUILD: {
                if (!IsWithinInteractionRange(playerGridX, playerGridY, action.gridX, action.gridY, 3)) return;

                int bID = map.tiles[targetIndex].buildingID;

                if (bID == -1) {
                    Systems::World::BuildingSystem::CreateGhostBuilding(map, action.buildType, action.gridX, action.gridY);
                    std::cout << "Santiye yerlestirildi! ID beklemede..." << std::endl;
                } else {
                    auto* building = Systems::World::BuildingSystem::GetBuilding(map, bID);
                    auto* coreBase = Systems::World::BuildingSystem::GetCoreBase(map);

                    if (building != nullptr && !building->isBuilt && coreBase != nullptr) {

                        building->timeSinceLastDeduction += dt;
                        bool canProgress = true;

                        // Her 1 saniyede bir (veya belirlediğin sürede) eksik malzemelerden 1 adet çekmeyi dene
                        if (building->timeSinceLastDeduction >= 1.0f) {
                            for (auto& pair : building->remainingCost) {
                                if (pair.second > 0) { // Eğer bu eşyadan hala gerekiyorsa
                                    // Üssün envanterinde var mı?
                                    if (coreBase->globalInventory[pair.first] > 0) {
                                        coreBase->globalInventory[pair.first]--; // Üsten düş
                                        pair.second--;                           // Şantiyedeki ihtiyacı azalt
                                        building->timeSinceLastDeduction = 0.0f; // Sayacı sıfırla
                                        std::cout << "Usten 1 adet malzeme cekildi!" << std::endl;
                                        break; // Bu tick'te sadece 1 eşya çek, sonrakini bekle
                                    } else {
                                        // Üste malzeme yok!
                                        canProgress = false;
                                        std::cout << "Uyari: Uste yeterli malzeme yok! Insaat durdu." << std::endl;
                                        break;
                                    }
                                }
                            }
                        }

                        // Eğer malzeme beklemiyorsak veya az önce başarılı çektiysek ilerlemeyi artır
                        if (canProgress) {
                            building->buildProgress += dt;

                            if (building->buildProgress >= building->maxBuildTime) {
                                building->isBuilt = true;
                                building->buildProgress = building->maxBuildTime;
                                std::cout << "Insaat TAMAMLANDI! ID: " << building->id << std::endl;
                            }
                        }
                    }
                }
                break;
            }
            case Data::EntityData::ActionType::DEMOLISH: {
                if (!IsWithinInteractionRange(playerGridX, playerGridY, action.gridX, action.gridY, 3)) return;

                int bID = map.tiles[targetIndex].buildingID;
                if (bID == -1) return;

                // Ana üssün silinmesini engelle
                auto* building = Systems::World::BuildingSystem::GetBuilding(map, bID);
                if (building != nullptr && building->type == Data::WorldData::BuildingType::CORE_BASE) {
                    std::cout << "Ana us yikilamaz!" << std::endl;
                    return;
                }

                map.tiles[targetIndex].buildingID = -1;
                std::cout << "Yapi yikildi! Koordinat: " << action.gridX << ", " << action.gridY << std::endl;
                break;
            }
            case Data::EntityData::ActionType::TRANSFER_BASE: {
                int bID = map.tiles[targetIndex].buildingID;
                if (bID > 0) {
                    auto* coreBase = Systems::World::BuildingSystem::GetCoreBase(map);
                    if (coreBase != nullptr && bID == coreBase->id) {
                        int totalTransferred = 0;

                        for (const auto& slot : player.inventory) {
                            if (slot.count > 0) {
                                // int olan itemID değerini tekrar Enum'a çeviriyoruz
                                auto itemType = static_cast<Data::WorldData::ItemType>(slot.itemID);
                                coreBase->globalInventory[itemType] += slot.count;
                                totalTransferred += slot.count;
                            }
                        }

                        if (totalTransferred > 0) {
                            player.inventory.clear();
                            std::cout << "Esyalar usse aktarildi! Toplam " << totalTransferred << " adet." << std::endl;
                        } else {
                            std::cout << "Uzerinde aktarilacak esya yok." << std::endl;
                        }
                    }
                }
                break;
            }
            default: break;
        }
    }
}