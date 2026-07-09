#pragma once

#include "../Data/UIData/BuildingMenuData.h"

namespace Systems::BuildingMenuSystem {

    // Processes menu interactions.
    // Returns true if the input was consumed by the UI.
    bool Update(Data::UIData::BuildingMenuState* state);

    // Loads all menu items for the selected category.
    void LoadCategoryItems(Data::UIData::BuildingMenuState* state);

} // namespace Systems::BuildingMenuSystem