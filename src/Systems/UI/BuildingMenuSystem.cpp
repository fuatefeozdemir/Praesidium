#include "../../../include/Systems/UI/BuildingMenuSystem.h"

namespace Systems::UI::BuildingMenuSystem {

    void LoadCategoryItems(Data::UIData::BuildingMenuState* state) {
        state->currentCategoryItems.clear();
        
        // Test için sadece lojistik ekli.
        if (state->activeCategory == Data::UIData::BuildingCategory::LOGISTICS) {
            Data::UIData::BuildingMenuItem conveyor;
            conveyor.type = Data::WorldData::BuildingType::CONVEYOR;
            conveyor.name = "Konveyor";
            conveyor.description = "Esyalari tasir.";
            // Maliyet: 2 Demir
            conveyor.cost[Data::WorldData::ItemType::IRON_ORE] = 2;
            
            // Butonun panel içindeki konumu
            conveyor.bounds = { state->panelBounds.x + 20, state->panelBounds.y + 60, 200, 40 };
            
            state->currentCategoryItems.push_back(conveyor);
        }
    }

bool Update(Data::UIData::BuildingMenuState* state) {
        Vector2 mousePos = GetMousePosition();

        // İkonu ekranın sağ alt köşesine dinamik olarak sabitle
        state->toggleButtonBounds.x = (float)GetScreenWidth() - state->toggleButtonBounds.width - 20;
        state->toggleButtonBounds.y = (float)GetScreenHeight() - state->toggleButtonBounds.height - 20;

        // 1. AÇMA/KAPAMA MANTIĞI: Sağ alttaki ikona sol tıklandı mı?
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mousePos, state->toggleButtonBounds)) {
                state->isOpen = !state->isOpen;
                if (state->isOpen) {
                    LoadCategoryItems(state);
                }
                return true;
            }
        }

        // 2. İPTAL MANTIĞI: İnşa modundan çık
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            state->selectedBuilding = Data::WorldData::BuildingType::NONE;
        }

        // Menü kapalıysa panelle ilgili tıklama hesaplamalarına girme
        if (!state->isOpen) return false;

        // 3. UI ANA PANEL ETKİLEŞİM ALANI
        if (CheckCollisionPointRec(mousePos, state->panelBounds)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

                Rectangle logisticsTab = { state->panelBounds.x + 20, state->panelBounds.y + 10, 80, 30 };
                if (CheckCollisionPointRec(mousePos, logisticsTab)) {
                    if (state->activeCategory != Data::UIData::BuildingCategory::LOGISTICS) {
                        state->activeCategory = Data::UIData::BuildingCategory::LOGISTICS;
                        LoadCategoryItems(state);
                    }
                }

                for (const auto& item : state->currentCategoryItems) {
                    if (CheckCollisionPointRec(mousePos, item.bounds)) {
                        state->selectedBuilding = item.type;
                        break;
                    }
                }
            }
            return true; // Ana panel tıklandığı için harita etkileşimini engelle
        }

        return false;
    }
}