#pragma once
#include <vector>
#include "../../Data/WorldData/MapData.h"
#include "../../Data/WorldData/BuildingData.h"

namespace Systems::World::BuildingSystem {
    extern std::vector<Data::WorldData::Building> activeBuildings;
    void CreateCoreBase(Data::WorldData::Map& map);
}