#pragma once

#include <vector>

#include "ItemData.h"

namespace Data::WorldData {

    // Defines the input, output, and processing time of a crafting recipe.
    struct Recipe {
        int id;
        std::vector<ItemAmount> inputs;
        std::vector<ItemAmount> outputs;
        int processingTime;
    };

}