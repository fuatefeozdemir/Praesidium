#pragma once

#include "AppState.h"
#include "../EntityData/PlayerData.h"
#include "../WorldData/MapData.h"
#include "../UIData/BuildingMenuData.h"

namespace Data::CoreData {

    // Central container that stores the complete runtime game state.
    // All gameplay systems operate on this shared context.
    struct GameContext {

        // ----- Game State -----
        AppState currentState = AppState::MAIN_MENU;

        // ----- Simulation -----
        Data::EntityData::Player player;
        Data::WorldData::Map worldMap;

        // ----- UI -----
        Data::UIData::BuildingMenuState buildingMenu;
    };

}