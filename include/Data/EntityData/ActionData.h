#pragma once
#include "../CoreData/Vector2Int.h"
#include "../WorldData/BuildingData.h"

namespace Data::EntityData {

    enum class ActionType {
        NONE,
        MINE_ORE,       // Maden kazma
        BUILD,          // Bina inşa etme
        DEMOLISH,       // Bina yıkma
        TRANSFER_BASE   // Üsse eşya aktarma
    };

    struct PlayerAction {
        int playerID;                            // Multiplayer için kimin yaptığı önemli
        ActionType type;
        Data::CoreData::Vector2Int targetPos;    // Hedef koordinat
        Data::WorldData::BuildingType buildType; // Eğer inşa ise hangi bina
    };
}