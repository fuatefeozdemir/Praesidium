#pragma once

#include "../Data/WorldData/BuildingData.h"
#include "../Data/WorldData/MapData.h"

namespace Systems::BuildingSystem {

    constexpr Data::WorldData::BuildingId INVALID_BUILDING_ID = -1;

    // Returns the building with the specified ID.
    // TODO: Replace linear lookup with direct ID indexing or a lookup table.
    Data::WorldData::Building* GetBuilding(
        Data::WorldData::Map& map,
        Data::WorldData::BuildingId id
    );

    // Validates whether a building can be placed at the specified position.
    bool CanPlaceBuilding(
        Data::WorldData::Map& map,
        const Data::WorldData::BuildingDefinition* definition,
        Data::CoreData::Vector2Int position
    );

    // Creates a new building and all required components.
    Data::WorldData::BuildingId CreateBuilding(
        Data::WorldData::Map& map,
        Data::WorldData::BuildingType type,
        Data::CoreData::Vector2Int position,
        Data::WorldData::Direction direction
    );

    // Removes a building from the world.
    void DestroyBuilding(
        Data::WorldData::Map& map,
        Data::WorldData::BuildingId id
    );

} // namespace Systems::BuildingSystem