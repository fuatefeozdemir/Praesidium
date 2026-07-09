#include "../../include/Systems/BuildingSystem.h"
#include "../../include/Systems/MapSystem.h"

namespace Systems::BuildingSystem {

    // Returns the building with the specified ID.
    // TODO: Replace linear lookup with direct ID indexing or a lookup table.
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

    // Validates whether a building can be placed at the specified position.
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

                if (tile && tile->buildingId != -1) {
                    return false;
                }
            }
        }

        return true;
    }

    // Creates a new building and allocates all required components.
    Data::WorldData::BuildingId CreateBuilding(
        Data::WorldData::Map& map,
        Data::WorldData::BuildingType type,
        Data::CoreData::Vector2Int position,
        Data::WorldData::Direction direction) {

        const auto* definition =
            Data::WorldData::GetBuildingDefinition(type);

        if (!definition) {
            return INVALID_BUILDING_ID;
        }

        if (!CanPlaceBuilding(map, definition, position)) {
            return INVALID_BUILDING_ID;
        }

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

        newBuilding.id = newId;
        newBuilding.type = type;
        newBuilding.position = position;
        newBuilding.size = definition->defaultSize;
        newBuilding.direction = direction;
        newBuilding.state = Data::WorldData::BuildingState::ACTIVE;
                if (definition->hasHealth) {

            const int componentIndex = map.healthCount++;

            map.healths[componentIndex].buildingId = newId;
            map.healths[componentIndex].currentHealth = definition->baseHealth;

            newBuilding.healthIndex = componentIndex;
        }

        if (definition->hasInventory) {

            const int componentIndex = map.inventoryCount++;

            map.inventories[componentIndex].buildingId = newId;

            newBuilding.inventoryIndex = componentIndex;
        }

        if (definition->hasProduction) {

            const int componentIndex = map.productionCount++;

            map.productions[componentIndex].buildingId = newId;

            newBuilding.productionIndex = componentIndex;
        }

        if (definition->hasExtractor) {

            const int componentIndex = map.extractorCount++;

            map.extractors[componentIndex].buildingId = newId;

            newBuilding.extractorIndex = componentIndex;
        }

        if (definition->hasPowerProducer) {

            const int componentIndex = map.powerProducerCount++;

            map.powerProducers[componentIndex].buildingId = newId;

            newBuilding.powerProducerIndex = componentIndex;
        }

        if (definition->hasPowerConsumer) {

            const int componentIndex = map.powerConsumerCount++;

            map.powerConsumers[componentIndex].buildingId = newId;

            newBuilding.powerConsumerIndex = componentIndex;
        }

        if (definition->hasConveyor) {

            const int componentIndex = map.conveyorCount++;

            map.conveyors[componentIndex].buildingId = newId;

            newBuilding.conveyorIndex = componentIndex;
        }

        if (definition->hasSplitter) {

            const int componentIndex = map.splitterCount++;

            map.splitters[componentIndex].buildingId = newId;

            newBuilding.splitterIndex = componentIndex;
        }

        // Occupy all tiles covered by the building.

        for (int y = 0; y < definition->defaultSize.y; ++y) {

            for (int x = 0; x < definition->defaultSize.x; ++x) {

                Data::CoreData::Vector2Int worldPos{
                    position.x + x,
                    position.y + y
                };

                MapSystem::SetTileBuildingID(map, worldPos, newId);
            }
        }

        return newId;
    }

    void DestroyBuilding(
        Data::WorldData::Map& map,
        Data::WorldData::BuildingId id) {

        (void)map;
        (void)id;
    }

} // namespace Systems::BuildingSystem