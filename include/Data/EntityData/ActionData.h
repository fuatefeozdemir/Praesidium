#pragma once
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
        int playerID;          // Multiplayer için kimin yaptığı önemli
        ActionType type;
        int gridX;             // Hedef X koordinatı
        int gridY;             // Hedef Y koordinatı
        Data::WorldData::BuildingType buildType; // Eğer inşa ise hangi bina
    };
}