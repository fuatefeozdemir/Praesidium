#include "../../../../include/Interface/UI/CoreUI/BuildingMenuRenderer.h"

namespace Interface::UI::CoreUI::BuildingMenuRenderer {

    void Draw(const Data::UIData::BuildingMenuState* state) {
        // 1. Sağ Alt Köşe: Menü Açma/Kapama Butonu
        Color toggleColor = state->isOpen ? RED : DARKGRAY;
        DrawRectangleRec(state->toggleButtonBounds, toggleColor);
        DrawRectangleLinesEx(state->toggleButtonBounds, 2.0f, LIGHTGRAY);
        DrawText(state->isOpen ? "KAPAT" : "MENU", 
                 (int)(state->toggleButtonBounds.x + 10), 
                 (int)(state->toggleButtonBounds.y + 20), 10, WHITE);

        // 2. İnşa Modu Aktif Uyarısı
        if (state->selectedBuilding != Data::WorldData::BuildingType::NONE) {
            DrawText("Insa Modu Aktif! (Iptal: Sag Tik)", GetScreenWidth() / 2 - 150, 20, 20, GREEN);
        }

        // Menü kapalıysa gerisini çizme
        if (!state->isOpen) return;

        // 3. Ana Panel
        DrawRectangleRec(state->panelBounds, Fade(DARKGRAY, 0.95f));
        DrawRectangleLinesEx(state->panelBounds, 2.0f, LIGHTGRAY);

        // 4. Kategori Sekmesi (Lojistik)
        Color logColor = (state->activeCategory == Data::UIData::BuildingCategory::LOGISTICS) ? GRAY : DARKBLUE;
        Rectangle logisticsTab = { state->panelBounds.x + 20, state->panelBounds.y + 10, 80, 30 };
        DrawRectangleRec(logisticsTab, logColor);
        DrawText("Lojistik", (int)(logisticsTab.x + 15), (int)(logisticsTab.y + 10), 10, WHITE);

        // 5. Binaların Listelenmesi
        for (const auto& item : state->currentCategoryItems) {
            Color btnColor = (state->selectedBuilding == item.type) ? GREEN : GRAY;
            
            DrawRectangleRec(item.bounds, btnColor);
            DrawRectangleLinesEx(item.bounds, 1.0f, BLACK);
            
            DrawText(item.name.c_str(), (int)(item.bounds.x + 10), (int)(item.bounds.y + 10), 20, WHITE);
        }
    }
}