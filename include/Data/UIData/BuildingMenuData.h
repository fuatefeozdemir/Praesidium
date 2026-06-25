#pragma once
#include "raylib.h"
#include <string>
#include <map>
#include <vector>
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

    // Menüdeki Tek Bir Bina Seçeneğinin Verisi
    struct BuildingMenuItem {
        Data::WorldData::BuildingType type;
        std::string name;
        std::string description;
        std::map<Data::WorldData::ItemType, int> cost; // Örn: {ItemType::IRON, 5}

        Rectangle bounds; // Tıklama ve çizim alanı
    };

    // Menü açık mı?
    struct BuildingMenuState {
        bool isOpen = false;
        BuildingCategory activeCategory = BuildingCategory::LOGISTICS;
        Data::WorldData::BuildingType selectedBuilding = Data::WorldData::BuildingType::NONE;

        Rectangle panelBounds = { 20, 20, 350, 500 }; // Tıklama tüketimi için ana panel boyutu
        Rectangle toggleButtonBounds = { 0, 0, 60, 60 }; // Sağ alt ikonun boyutu

        // Seçili kategoriye ait ekranda gösterilecek binaların listesi
        std::vector<BuildingMenuItem> currentCategoryItems;
    };
}