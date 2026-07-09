#pragma once

#include "../CoreData/Vector2Int.h"
#include "../WorldData/BuildingData.h"

namespace Data::EntityData {

    enum class ActionType {
        NONE,
        MINE_ORE,
        BUILD,
        DEMOLISH,
        TRANSFER_BASE
    };

    struct PlayerAction {
        // TODO: Replace playerID with an EntityId when multiplayer support is implemented.
        int playerID = 0;

        ActionType type = ActionType::NONE;

        // Target tile
        Data::CoreData::Vector2Int targetPos = {0, 0};

        // Building type for BUILD action
        Data::WorldData::BuildingType buildType = Data::WorldData::BuildingType::NONE;
    };

} // namespace Data::EntityData