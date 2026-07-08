#pragma once
#include "../../Data/WorldData/MapData.h"

namespace Interface::World::MapRenderer {

    // Her frame çağrılacak ana çizim fonksiyonu
    void Draw(const Data::WorldData::Map& map);

}