#include "../../include/Systems/BuildingSystem.h"
#include "../../include/Systems/MapSystem.h"

namespace {

    constexpr int INVALID_BUILDING_ID = -1;

    // Helper template to safely swap & pop any ECS component and update the owner's index.
    template <typename T>
    void SwapAndPopComponent(
        Data::WorldData::Map& map,
        int delIdx,
        T* componentArray,
        int& count,
        int Data::WorldData::Building::* indexField) {

        if (delIdx == -1) return;

        int lastIdx = count - 1;
        componentArray[delIdx] = componentArray[lastIdx];
        componentArray[lastIdx] = {}; // Clear ghost data
        count--;

        if (delIdx != lastIdx) {
            auto* owner = Systems::BuildingSystem::GetBuilding(map, componentArray[delIdx].buildingId);
            if (owner) {
                owner->*indexField = delIdx;
            }
        }
    }

    void UpdateConveyorConnections(
        Data::WorldData::Map& map,
        Data::WorldData::Building& building) {

        auto& conveyor = map.conveyors[building.conveyorIndex];

        Data::CoreData::Vector2Int frontPos = building.position;
        Data::CoreData::Vector2Int backPos = building.position;

        switch (building.direction) {
            case Data::WorldData::Direction::NORTH:
                frontPos.y -= 1;
                backPos.y += 1;
                break;
            case Data::WorldData::Direction::EAST:
                frontPos.x += 1;
                backPos.x -= 1;
                break;
            case Data::WorldData::Direction::SOUTH:
                frontPos.y += 1;
                backPos.y -= 1;
                break;
            case Data::WorldData::Direction::WEST:
                frontPos.x -= 1;
                backPos.x += 1;
                break;
        }

        // 1. Check front tile for outgoing connection
        auto* frontTile = Systems::MapSystem::GetTile(map, frontPos);
        if (frontTile && frontTile->buildingId != INVALID_BUILDING_ID && frontTile->buildingId != building.id) {

            auto* frontBuilding = Systems::BuildingSystem::GetBuilding(map, frontTile->buildingId);

            if (frontBuilding) {
                // TODO: Add Direction Validations here (Does frontBuilding accept input from this side?)
                conveyor.nextBuildingId = frontBuilding->id;

                if (frontBuilding->conveyorIndex != -1) {
                    map.conveyors[frontBuilding->conveyorIndex].previousBuildingId = building.id;
                }
            }
        }

        // 2. Check back tile for incoming connection
        auto* backTile = Systems::MapSystem::GetTile(map, backPos);
        if (backTile && backTile->buildingId != INVALID_BUILDING_ID && backTile->buildingId != building.id) {

            auto* backBuilding = Systems::BuildingSystem::GetBuilding(map, backTile->buildingId);

            if (backBuilding) {
                // TODO: Add Direction Validations here (Does backBuilding output to this side?)
                conveyor.previousBuildingId = backBuilding->id;

                if (backBuilding->conveyorIndex != -1) {
                    map.conveyors[backBuilding->conveyorIndex].nextBuildingId = building.id;
                }
            }
        }
    }

    // Severs connections TO the destroyed building from ANY conveyor.
    // Prevents dangling references when machines (Furnace, Miner, etc.) are destroyed.
    void DisconnectNeighbourBuildings(Data::WorldData::Map& map, Data::WorldData::BuildingId destroyedId) {

        // TODO: Optimize this O(N) loop when a formal connection graph is introduced.
        for (int i = 0; i < map.conveyorCount; ++i) {
            if (map.conveyors[i].nextBuildingId == destroyedId) {
                map.conveyors[i].nextBuildingId = INVALID_BUILDING_ID;
            }
            if (map.conveyors[i].previousBuildingId == destroyedId) {
                map.conveyors[i].previousBuildingId = INVALID_BUILDING_ID;
            }
        }
    }

} // namespace

namespace Systems::BuildingSystem {

    Data::WorldData::Building* GetBuilding(
        Data::WorldData::Map& map,
        Data::WorldData::BuildingId id) {

        for (int i = 0; i < map.buildingCount; ++i) {
            if (map.buildings[i].id == id) {
                return &map.buildings[i];
            }
        }

        return nullptr;
    }

    bool CanPlaceBuilding(
        Data::WorldData::Map& map,
        const Data::WorldData::BuildingDefinition* definition,
        Data::CoreData::Vector2Int position) {

        for (int y = 0; y < definition->defaultSize.y; ++y) {

            for (int x = 0; x < definition->defaultSize.x; ++x) {

                Data::CoreData::Vector2Int worldPos{
                    position.x + x,
                    position.y + y
                };

                const auto chunkPos = MapSystem::WorldToChunk(worldPos);

                if (!MapSystem::IsInsideWorld(map, chunkPos)) {
                    return false;
                }

                auto* tile = MapSystem::GetTile(map, worldPos);

                if (tile && tile->buildingId != INVALID_BUILDING_ID) {
                    return false;
                }
            }
        }

        return true;
    }

    Data::WorldData::BuildingId CreateBuilding(
        Data::WorldData::Map& map,
        Data::WorldData::BuildingType type,
        Data::CoreData::Vector2Int position,
        Data::WorldData::Direction direction) {

        const auto* definition = Data::WorldData::GetBuildingDefinition(type);

        if (!definition) return INVALID_BUILDING_ID;
        if (!CanPlaceBuilding(map, definition, position)) return INVALID_BUILDING_ID;

        if (map.buildingCount >= Data::WorldData::MAX_BUILDINGS) return INVALID_BUILDING_ID;
        if (definition->hasHealth && map.healthCount >= Data::WorldData::MAX_BUILDINGS) return INVALID_BUILDING_ID;
        if (definition->hasInventory && map.inventoryCount >= Data::WorldData::MAX_INVENTORIES) return INVALID_BUILDING_ID;
        if (definition->hasProduction && map.productionCount >= Data::WorldData::MAX_PRODUCTIONS) return INVALID_BUILDING_ID;
        if (definition->hasExtractor && map.extractorCount >= Data::WorldData::MAX_EXTRACTORS) return INVALID_BUILDING_ID;
        if (definition->hasPowerProducer && map.powerProducerCount >= Data::WorldData::MAX_POWER_COMPONENTS) return INVALID_BUILDING_ID;
        if (definition->hasPowerConsumer && map.powerConsumerCount >= Data::WorldData::MAX_POWER_COMPONENTS) return INVALID_BUILDING_ID;
        if (definition->hasConveyor && map.conveyorCount >= Data::WorldData::MAX_CONVEYORS) return INVALID_BUILDING_ID;
        if (definition->hasSplitter && map.splitterCount >= Data::WorldData::MAX_SPLITTERS) return INVALID_BUILDING_ID;

        const Data::WorldData::BuildingId newId = map.nextBuildingId++;
        const int buildingIndex = map.buildingCount++;
        auto& newBuilding = map.buildings[buildingIndex];

        newBuilding = {}; // Ensure clean state
        newBuilding.id = newId;
        newBuilding.type = type;
        newBuilding.position = position;
        newBuilding.size = definition->defaultSize;
        newBuilding.direction = direction;
        newBuilding.state = Data::WorldData::BuildingState::ACTIVE;

        newBuilding.healthIndex = -1;
        newBuilding.inventoryIndex = -1;
        newBuilding.productionIndex = -1;
        newBuilding.extractorIndex = -1;
        newBuilding.powerProducerIndex = -1;
        newBuilding.powerConsumerIndex = -1;
        newBuilding.conveyorIndex = -1;
        newBuilding.splitterIndex = -1;

        if (definition->hasHealth) {
            const int componentIndex = map.healthCount++;
            map.healths[componentIndex] = {};
            map.healths[componentIndex].buildingId = newId;
            map.healths[componentIndex].currentHealth = definition->baseHealth;
            newBuilding.healthIndex = componentIndex;
        }

        if (definition->hasInventory) {
            const int componentIndex = map.inventoryCount++;
            map.inventories[componentIndex] = {};
            map.inventories[componentIndex].buildingId = newId;
            newBuilding.inventoryIndex = componentIndex;
        }

        if (definition->hasProduction) {
            const int componentIndex = map.productionCount++;
            map.productions[componentIndex] = {};
            map.productions[componentIndex].buildingId = newId;
            newBuilding.productionIndex = componentIndex;
        }

        if (definition->hasExtractor) {
            const int componentIndex = map.extractorCount++;
            map.extractors[componentIndex] = {};
            map.extractors[componentIndex].buildingId = newId;
            newBuilding.extractorIndex = componentIndex;
        }

        if (definition->hasPowerProducer) {
            const int componentIndex = map.powerProducerCount++;
            map.powerProducers[componentIndex] = {};
            map.powerProducers[componentIndex].buildingId = newId;
            newBuilding.powerProducerIndex = componentIndex;
        }

        if (definition->hasPowerConsumer) {
            const int componentIndex = map.powerConsumerCount++;
            map.powerConsumers[componentIndex] = {};
            map.powerConsumers[componentIndex].buildingId = newId;
            newBuilding.powerConsumerIndex = componentIndex;
        }

        if (definition->hasConveyor) {
            const int componentIndex = map.conveyorCount++;
            map.conveyors[componentIndex] = {};
            map.conveyors[componentIndex].buildingId = newId;
            newBuilding.conveyorIndex = componentIndex;
        }

        if (definition->hasSplitter) {
            const int componentIndex = map.splitterCount++;
            map.splitters[componentIndex] = {};
            map.splitters[componentIndex].buildingId = newId;
            newBuilding.splitterIndex = componentIndex;
        }

        for (int y = 0; y < definition->defaultSize.y; ++y) {
            for (int x = 0; x < definition->defaultSize.x; ++x) {
                Data::CoreData::Vector2Int worldPos{position.x + x, position.y + y};
                MapSystem::SetTileBuildingID(map, worldPos, newId);
            }
        }

        if (definition->hasConveyor) {
            UpdateConveyorConnections(map, newBuilding);
        }

        return newId;
    }

    void DestroyBuilding(
        Data::WorldData::Map& map,
        Data::WorldData::BuildingId id) {

        auto* building = GetBuilding(map, id);
        if (!building) return;

        int deletedBuildingIndex = static_cast<int>(building - map.buildings);

        // Disconnect logic linkages safely (Works for both machines and belts)
        DisconnectNeighbourBuildings(map, id);

        // Free up the tiles directly using building->size
        for (int y = 0; y < building->size.y; ++y) {
            for (int x = 0; x < building->size.x; ++x) {
                Data::CoreData::Vector2Int worldPos{building->position.x + x, building->position.y + y};
                MapSystem::SetTileBuildingID(map, worldPos, INVALID_BUILDING_ID);
            }
        }

        // SWAP & POP: All Components
        SwapAndPopComponent(map, building->conveyorIndex, map.conveyors, map.conveyorCount, &Data::WorldData::Building::conveyorIndex);
        SwapAndPopComponent(map, building->healthIndex, map.healths, map.healthCount, &Data::WorldData::Building::healthIndex);
        SwapAndPopComponent(map, building->inventoryIndex, map.inventories, map.inventoryCount, &Data::WorldData::Building::inventoryIndex);
        SwapAndPopComponent(map, building->productionIndex, map.productions, map.productionCount, &Data::WorldData::Building::productionIndex);
        SwapAndPopComponent(map, building->extractorIndex, map.extractors, map.extractorCount, &Data::WorldData::Building::extractorIndex);
        SwapAndPopComponent(map, building->powerProducerIndex, map.powerProducers, map.powerProducerCount, &Data::WorldData::Building::powerProducerIndex);
        SwapAndPopComponent(map, building->powerConsumerIndex, map.powerConsumers, map.powerConsumerCount, &Data::WorldData::Building::powerConsumerIndex);
        SwapAndPopComponent(map, building->splitterIndex, map.splitters, map.splitterCount, &Data::WorldData::Building::splitterIndex);

        // SWAP & POP: Main Building Array
        int lastBuildingIdx = map.buildingCount - 1;
        map.buildings[deletedBuildingIndex] = map.buildings[lastBuildingIdx];
        map.buildings[lastBuildingIdx] = {}; // Clear ghost building
        map.buildingCount--;
    }

} // namespace Systems::BuildingSystem