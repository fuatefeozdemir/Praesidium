#pragma once
#include "../Data/UIData/BuildingMenuData.h"

namespace Systems::BuildingMenuSystem {
    // Menü etkileşimlerini işler. Tıklama UI üzerinde gerçekleşirse true döner.
    bool Update(Data::UIData::BuildingMenuState* state);
    
    // Aktif kategoriye göre listedeki bina butonlarını yükler
    void LoadCategoryItems(Data::UIData::BuildingMenuState* state);
}