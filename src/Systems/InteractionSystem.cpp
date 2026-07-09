#include "../../include/Systems/InteractionSystem.h"

#include <cmath>
#include <iostream>

#include "../../include/Systems/BuildingSystem.h"
#include "../../include/Systems/MapSystem.h"

namespace {

    constexpr int MINE_RANGE = 2;
    constexpr int BUILD_RANGE = 3;
    constexpr int MINE_DURATION = 60;

    bool IsWithinInteractionRange(
        Data::CoreData::Vector2Int playerTile,
        Data::CoreData::Vector2Int targetTile,
        int maxRange) {

        return std::abs(playerTile.x - targetTile.x) <= maxRange &&
               std::abs(playerTile.y - targetTile.y) <= maxRange;
    }

} // namespace

namespace Systems::InteractionSystem {

    void ExecuteActionOnServer(
        Data::EntityData::Player& player,
        Data::WorldData::Map& map,
        const Data::EntityData::PlayerAction& action) {

        const auto targetTile = action.targetPos;
        const auto chunkPos = Systems::MapSystem::WorldToChunk(targetTile);

        if (!Systems::MapSystem::IsInsideWorld(map, chunkPos)) {
            return;
        }

        const auto playerTile =
            Systems::MapSystem::PixelToTile(player.position.x, player.position.y);

        if (player.lastTargetX != targetTile.x ||
            player.lastTargetY != targetTile.y) {

            player.actionTimer = 0;
            player.lastTargetX = targetTile.x;
            player.lastTargetY = targetTile.y;
        }

        auto* tile = Systems::MapSystem::GetOrCreateTile(map, targetTile);

        if (!tile) {
            return;
        }

        switch (action.type) {

            case Data::EntityData::ActionType::MINE_ORE: {

                if (!IsWithinInteractionRange(playerTile, targetTile, MINE_RANGE)) {
                    return;
                }

                if (tile->ore == Data::WorldData::ItemType::NONE) {
                    return;
                }

                player.actionTimer++;

                if (player.actionTimer < MINE_DURATION) {
                    return;
                }

                player.actionTimer = 0;

                bool found = false;

                for (auto& slot : player.inventory) {
                    if (slot.type == tile->ore) {
                        slot.amount++;
                        found = true;
                        break;
                    }
                }

                if (!found &&
                    player.inventory.size() < static_cast<size_t>(player.maxSlots)) {

                    player.inventory.push_back({
                        .type = tile->ore,
                        .amount = 1
                    });
                }

                // TODO: Remove ore from the world once resource depletion is implemented.

                std::cout << "Ore mined." << std::endl;

                break;
            }

            case Data::EntityData::ActionType::BUILD: {

                if (!IsWithinInteractionRange(playerTile, targetTile, BUILD_RANGE)) {
                    return;
                }

                if (tile->buildingId != -1) {
                    std::cout << "A building already exists here." << std::endl;
                    return;
                }

                const auto id = Systems::BuildingSystem::CreateBuilding(
                    map,
                    action.buildType,
                    targetTile,
                    Data::WorldData::Direction::NORTH
                );

                if (id != -1) {
                    std::cout << "Building created. ID: " << id << std::endl;
                } else {
                    std::cout << "Building placement failed." << std::endl;
                }

                break;
            }

            case Data::EntityData::ActionType::DEMOLISH: {

                if (!IsWithinInteractionRange(playerTile, targetTile, BUILD_RANGE)) {
                    return;
                }

                if (tile->buildingId == -1) {
                    return;
                }

                Systems::BuildingSystem::DestroyBuilding(map, tile->buildingId);

                std::cout << "Building destroyed." << std::endl;

                break;
            }

            case Data::EntityData::ActionType::TRANSFER_BASE: {

                std::cout << "TRANSFER_BASE is not implemented yet." << std::endl;

                break;
            }

            default:
                break;
        }
    }

} // namespace Systems::InteractionSystem