#pragma once
#include <vector>
#include "../Data/WorldData/MapData.h"
#include "../Data/WorldData/BuildingData.h"

namespace Systems::BuildingSystem {
    Data::WorldData::Building* GetBuilding(Data::WorldData::Map& map, int id);
    Data::WorldData::Building* GetCoreBase(Data::WorldData::Map& map);
    void CreateGhostBuilding(Data::WorldData::Map& map, Data::WorldData::BuildingType type, int gridX, int gridY);
    void CreateCoreBase(Data::WorldData::Map& map);
}