#pragma once

#include "../Data/WorldData/MapData.h"
#include "../Data/WorldData/BuildingData.h"

namespace Systems::BuildingSystem {

    Data::WorldData::Building* GetBuilding(
        Data::WorldData::Map& map,
        Data::WorldData::BuildingId id
    );

    bool CanPlaceBuilding(
        Data::WorldData::Map& map,
        const Data::WorldData::BuildingDefinition* def,
        Data::CoreData::Vector2Int position
    );

    Data::WorldData::BuildingId CreateBuilding(
        Data::WorldData::Map& map,
        Data::WorldData::BuildingType type,
        Data::CoreData::Vector2Int position,
        Data::WorldData::Direction direction
    );

    void DestroyBuilding(
        Data::WorldData::Map& map,
        Data::WorldData::BuildingId id
    );

}