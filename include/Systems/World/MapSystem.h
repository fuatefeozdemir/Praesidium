#pragma once
#include "../../Data/WorldData/MapData.h"

namespace Systems::World::MapSystem {
    void Initialize(Data::WorldData::Map& map, int width, int height, int tileSize);
}