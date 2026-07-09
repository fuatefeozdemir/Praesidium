#pragma once

#include "../../Data/WorldData/MapData.h"

namespace Interface::World::MapRenderer {

    // Draws the entire visible world.
    void Draw(const Data::WorldData::Map& map);

} // namespace Interface::World::MapRenderer