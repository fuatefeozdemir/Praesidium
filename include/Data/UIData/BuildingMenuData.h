#pragma once

#include <map>
#include <string>
#include <vector>

#include "raylib.h"

#include "../WorldData/BuildingData.h"
#include "../WorldData/ItemData.h"

namespace Data::UIData {

    enum class BuildingCategory {
        EXTRACTION,
        LOGISTICS,
        PRODUCTION,
        POWER,
        DEFENSE
    };

    struct BuildingMenuItem {
        Data::WorldData::BuildingType type = Data::WorldData::BuildingType::NONE;

        std::string name;
        std::string description;

        // Building construction cost
        std::map<Data::WorldData::ItemType, int> cost;

        // UI bounds
        Rectangle bounds = {0.0f, 0.0f, 0.0f, 0.0f};

        // TODO: Consider replacing std::map with std::unordered_map
        // if item ordering is not required.
    };

    struct BuildingMenuState {
        bool isOpen = false;

        BuildingCategory activeCategory = BuildingCategory::LOGISTICS;
        Data::WorldData::BuildingType selectedBuilding = Data::WorldData::BuildingType::NONE;

        // Main panel bounds
        Rectangle panelBounds = {20.0f, 20.0f, 350.0f, 500.0f};

        // Toggle button bounds
        Rectangle toggleButtonBounds = {0.0f, 0.0f, 60.0f, 60.0f};

        // Visible buildings in the active category
        std::vector<BuildingMenuItem> currentCategoryItems;
    };

} // namespace Data::UIData