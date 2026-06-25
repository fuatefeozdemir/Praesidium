#include "../../../include/Interface/UI/BuildingMenuRenderer.h"
#include "../../../cmake-build-debug/_deps/raylib-src/src/raylib.h"

namespace Interface::UI::BuildingMenuRenderer {
    void Draw(const Data::CoreData::GameContext* context) {
        const auto* menuState = &context->buildingMenu;
        // 1. Sağ Alt Köşe: Menü Açma/Kapama Butonu
        Color toggleColor = menuState->isOpen ? RED : DARKGRAY;
        DrawRectangleRec(menuState->toggleButtonBounds, toggleColor);
        DrawRectangleLinesEx(menuState->toggleButtonBounds, 2.0f, LIGHTGRAY);
        DrawText(menuState->isOpen ? "KAPAT" : "MENU",
                 (int)(menuState->toggleButtonBounds.x + 10),
                 (int)(menuState->toggleButtonBounds.y + 20), 10, WHITE);

        // 2. İnşa Modu Aktif Uyarısı
        if (menuState->selectedBuilding != Data::WorldData::BuildingType::NONE) {
            DrawText("Insa Modu Aktif! (Iptal: Sag Tik)", GetScreenWidth() / 2 - 150, 20, 20, GREEN);
        }

        // Menü kapalıysa gerisini çizme
        if (!menuState->isOpen) return;

        // 3. Ana Panel
        DrawRectangleRec(menuState->panelBounds, Fade(DARKGRAY, 0.95f));
        DrawRectangleLinesEx(menuState->panelBounds, 2.0f, LIGHTGRAY);

        // 4. Kategori Sekmesi (Lojistik)
        Color logColor = (menuState->activeCategory == Data::UIData::BuildingCategory::LOGISTICS) ? GRAY : DARKBLUE;
        Rectangle logisticsTab = { menuState->panelBounds.x + 20, menuState->panelBounds.y + 10, 80, 30 };
        DrawRectangleRec(logisticsTab, logColor);
        DrawText("Lojistik", (int)(logisticsTab.x + 15), (int)(logisticsTab.y + 10), 10, WHITE);

        // 5. Binaların Listelenmesi
        for (const auto& item : menuState->currentCategoryItems) {
            Color btnColor = (menuState->selectedBuilding == item.type) ? GREEN : GRAY;

            DrawRectangleRec(item.bounds, btnColor);
            DrawRectangleLinesEx(item.bounds, 1.0f, BLACK);

            DrawText(item.name.c_str(), (int)(item.bounds.x + 10), (int)(item.bounds.y + 10), 20, WHITE);
        }
    }
}
