#pragma once
#include "../../Data/EntityData/PlayerData.h"
#include "../../Data/WorldData/MapData.h"
#include "../../Data/UIData/BuildingMenuData.h"

namespace Systems::Simulation::PlayerSystem {
    void Update(Data::EntityData::Player& player, Data::WorldData::Map& map, Data::UIData::BuildingMenuState* uiState);
}