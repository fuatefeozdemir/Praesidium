#include "../../include/Systems/InteractionSystem.h"
#include "../../include/Systems/MapSystem.h"
#include "../../include/Systems/BuildingSystem.h"
#include <cmath>
#include <iostream>

namespace Systems::InteractionSystem {

    bool IsWithinInteractionRange(Data::CoreData::Vector2Int playerTile, Data::CoreData::Vector2Int targetTile, int maxRange) {
        return (std::abs(playerTile.x - targetTile.x) <= maxRange) && (std::abs(playerTile.y - targetTile.y) <= maxRange);
    }

    void ExecuteActionOnServer(Data::EntityData::Player& player, Data::WorldData::Map& map, const Data::EntityData::PlayerAction& action) {
        Data::CoreData::Vector2Int targetTile = action.targetPos;
        Data::CoreData::Vector2Int chunkPos = Systems::MapSystem::WorldToChunk(targetTile);

        // 1. Dünya sınırları kontrolü
        if (!Systems::MapSystem::IsInsideWorld(map, chunkPos)) return;

        // 2. Oyuncunun Tile pozisyonunu hesapla
        Data::CoreData::Vector2Int playerTile = Systems::MapSystem::PixelToTile(player.position.x, player.position.y);

        // 3. Oyuncu hedefini değiştirirse sayacı sıfırla (Header'daki eski isimlendirme kullanıldı)
        if (player.lastTargetX != targetTile.x || player.lastTargetY != targetTile.y) {
            player.actionTimer = 0;
            player.lastTargetX = targetTile.x;
            player.lastTargetY = targetTile.y;
        }

        // 4. Hedef Tile'ı getir
        Data::WorldData::Tile* tile = Systems::MapSystem::GetOrCreateTile(map, targetTile);
        if (!tile) return;

        switch (action.type) {
            case Data::EntityData::ActionType::MINE_ORE: {
                if (!IsWithinInteractionRange(playerTile, targetTile, 2)) return;

                auto targetOre = tile->ore;
                if (targetOre != Data::WorldData::ItemType::NONE) {
                    player.actionTimer++;
                    if (player.actionTimer >= 60) {

                        Data::WorldData::ItemType minedItem = targetOre;
                        int itemID = static_cast<int>(minedItem);
                        bool itemFound = false;

                        for (auto& slot : player.inventory) {
                            if (slot.itemID == itemID) {
                                slot.count++;
                                std::cout << "Maden kazildi! Envanterdeki sayisi: " << slot.count << std::endl;
                                itemFound = true;
                                break;
                            }
                        }

                        if (!itemFound) {
                            if (player.inventory.size() < player.maxSlots) {
                                player.inventory.push_back({itemID, 1});
                                std::cout << "Maden kazildi! Envanterdeki sayisi: 1" << std::endl;
                            } else {
                                std::cout << "Envanter dolu! Maden alinamadi." << std::endl;
                            }
                        }
                        player.actionTimer = 0;
                    }
                }
                break;
            }
            case Data::EntityData::ActionType::BUILD: {
                if (!IsWithinInteractionRange(playerTile, targetTile, 3)) return;

                int bID = tile->buildingID;

                if (bID == -1) {
                    // Header'da CreateBuilding olmadığı için mevcut olan kullanıldı
                    Systems::BuildingSystem::CreateGhostBuilding(map, action.buildType, targetTile.x, targetTile.y);
                    std::cout << "Santiye yerlestirildi! ID beklemede..." << std::endl;
                } else {
                    std::cout << "Burada zaten bir bina var!" << std::endl;
                }
                break;
            }
            case Data::EntityData::ActionType::DEMOLISH: {
                if (!IsWithinInteractionRange(playerTile, targetTile, 3)) return;

                int bID = tile->buildingID;
                if (bID == -1) return;

                // CORE_BASE enum'da olmadığı için ID üzerinden kontrol yapıldı
                auto* coreBase = Systems::BuildingSystem::GetCoreBase(map);
                if (coreBase != nullptr && coreBase->id == bID) {
                    std::cout << "Ana us yikilamaz!" << std::endl;
                    return;
                }

                // Header'da DestroyBuilding olmadığı için manuel sıfırlama yapıldı
                tile->buildingID = -1;
                std::cout << "Yapi yikildi! Koordinat: " << targetTile.x << ", " << targetTile.y << std::endl;
                break;
            }
            case Data::EntityData::ActionType::TRANSFER_BASE: {
                int bID = tile->buildingID;
                if (bID > 0) {
                    auto* coreBase = Systems::BuildingSystem::GetCoreBase(map);

                    if (coreBase != nullptr && bID == coreBase->id) {
                        int totalTransferred = 0;

                        for (const auto& slot : player.inventory) {
                            if (slot.count > 0) {
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