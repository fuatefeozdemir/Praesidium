#pragma once
#include "../../Data/EntityData/PlayerData.h"
#include "../../Data/WorldData/MapData.h"

namespace Systems::Simulation::PlayerSystem {
    void Update(Data::EntityData::Player& player, Data::WorldData::Map& map);
}