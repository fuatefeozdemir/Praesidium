#pragma once
#include "AppState.h"
#include "../EntityData/PlayerData.h"
#include "../WorldData/MapData.h"
#include "../UIData/BuildingMenuData.h"

namespace Data::CoreData {
    struct GameContext {
        // Oyunun Genel Durumu
        AppState currentState = AppState::MAIN_MENU;

        // Simülasyon Verileri
        Data::EntityData::Player player;
        Data::WorldData::Map worldMap;

        // Arayüz Verileri
        Data::UIData::BuildingMenuState buildingMenu;
    };
}