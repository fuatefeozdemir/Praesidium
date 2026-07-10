#pragma once

#include "../../Data/EntityData/PlayerData.h"

namespace Interface::World::PlayerRenderer {

    // Draws the player entity in the world space.
    void Draw(const Data::EntityData::Player& player);

} // namespace Interface::World::PlayerRenderer