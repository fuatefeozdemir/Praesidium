#pragma once

namespace Data::WorldData {

    // Defines every item available in the game.
    enum class ItemType {
        NONE,

        IRON_ORE,
        COPPER_ORE,
        COAL,
        WATER,

        IRON_INGOT,
        COPPER_INGOT,

        MAX_ITEM_COUNT // Must remain the last entry.
    };

    struct ItemAmount {
        ItemType type;
        int amount;
    };

}