#pragma once
#include <vector>
#include "../../Data/WorldData/MapData.h"
#include "../../Data/WorldData/BuildingData.h"

namespace Systems::World::BuildingSystem {
    extern std::vector<Data::WorldData::Building> activeBuildings;
    Data::WorldData::Building* GetBuilding(int id);
    Data::WorldData::Building* GetCoreBase();
    void CreateCoreBase(Data::WorldData::Map& map);
    void CreateGhostBuilding(Data::WorldData::Map& map, Data::WorldData::BuildingType type, int gridX, int gridY);
}