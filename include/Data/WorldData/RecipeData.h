#pragma once
#include <vector>
#include "ItemData.h"

namespace Data::WorldData {
    struct Recipe {
        int id;
        std::vector<ItemAmount> inputs;
        std::vector<ItemAmount> outputs;
        float processingTime;
    };
}