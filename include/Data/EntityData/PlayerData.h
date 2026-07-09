#pragma once

#include <string>
#include <vector>

#include "../CoreData/Vector2Int.h"
#include "../WorldData/ItemData.h"

namespace Data::EntityData {

    // TODO: Move InventorySlot to a shared InventoryData module.
    // This structure will also be used by buildings, storage containers, and vehicles.
    struct InventorySlot {
        Data::WorldData::ItemType type = Data::WorldData::ItemType::NONE;
        int amount = 0;
    };

    enum class PlayerActionState {
        IDLE,
        MOVING,
        BUILDING
    };

    struct Player {
        // Identity
        std::string name;

        // Movement
        Data::CoreData::Vector2Int position;
        int speed = 0;
        int collisionRadius = 0;

        // Stats
        PlayerActionState state = PlayerActionState::IDLE;
        int health = 0;
        int maxHealth = 0;

        // Inventory
        int maxSlots = 0;
        std::vector<InventorySlot> inventory;

        // Interaction
        int actionTimer = 0;
        int lastTargetX = -1;
        int lastTargetY = -1;
    };

} // namespace Data::EntityData