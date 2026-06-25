#pragma once
#include "../Data/EntityData/ActionData.h"
#include "../Data/WorldData/MapData.h"
#include "../Data/EntityData/PlayerData.h"

namespace Systems::InteractionSystem {
    void ExecuteActionOnServer(Data::EntityData::Player& player, Data::WorldData::Map& map, const Data::EntityData::PlayerAction& action, float dt);
}