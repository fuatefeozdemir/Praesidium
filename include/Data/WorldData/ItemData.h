#pragma once

namespace Data::WorldData {
    enum class ItemType { 
        NONE, 
        IRON_ORE, 
        COPPER_ORE, 
        COAL, 
        IRON_INGOT, 
        COPPER_INGOT,
        WATER,

        MAX_ITEM_COUNT // sonda olmalı
    };

    struct ItemAmount {
        ItemType type;
        int amount;
    };
}