#include "../../../include/Interface/UI/BuildingMenuRenderer.h"

#include "raylib.h"

namespace {

    constexpr float PANEL_BORDER_THICKNESS = 2.0f;
    constexpr float BUTTON_BORDER_THICKNESS = 1.0f;

    constexpr int TITLE_FONT_SIZE = 20;
    constexpr int SMALL_FONT_SIZE = 10;

} // namespace

namespace Interface::UI::BuildingMenuRenderer {

    void Draw(const Data::CoreData::GameContext* context) {

        const auto& menu = context->buildingMenu;

        // Toggle Button

        const Color toggleColor =
            menu.isOpen ? RED : DARKGRAY;

        DrawRectangleRec(menu.toggleButtonBounds, toggleColor);

        DrawRectangleLinesEx(
            menu.toggleButtonBounds,
            PANEL_BORDER_THICKNESS,
            LIGHTGRAY
        );

        DrawText(
            menu.isOpen ? "CLOSE" : "MENU",
            static_cast<int>(menu.toggleButtonBounds.x + 10),
            static_cast<int>(menu.toggleButtonBounds.y + 20),
            SMALL_FONT_SIZE,
            WHITE
        );

        // Build Mode Indicator

        if (menu.selectedBuilding != Data::WorldData::BuildingType::NONE) {

            DrawText(
                "Build Mode Active (Right Click to Cancel)",
                GetScreenWidth() / 2 - 170,
                20,
                TITLE_FONT_SIZE,
                GREEN
            );
        }

        if (!menu.isOpen) {
            return;
        }

        // Main Panel

        DrawRectangleRec(
            menu.panelBounds,
            Fade(DARKGRAY, 0.95f)
        );

        DrawRectangleLinesEx(
            menu.panelBounds,
            PANEL_BORDER_THICKNESS,
            LIGHTGRAY
        );

        // TODO:
        // Replace the hardcoded logistics tab with a dynamic category system.

        const Rectangle logisticsTab = {
            menu.panelBounds.x + 20,
            menu.panelBounds.y + 10,
            80,
            30
        };

        const Color tabColor =
            menu.activeCategory == Data::UIData::BuildingCategory::LOGISTICS
            ? GRAY
            : DARKBLUE;

        DrawRectangleRec(logisticsTab, tabColor);

        DrawText(
            "Logistics",
            static_cast<int>(logisticsTab.x + 10),
            static_cast<int>(logisticsTab.y + 10),
            SMALL_FONT_SIZE,
            WHITE
        );

        // Building Buttons

        for (const auto& item : menu.currentCategoryItems) {

            const Color buttonColor =
                menu.selectedBuilding == item.type
                ? GREEN
                : GRAY;

            DrawRectangleRec(item.bounds, buttonColor);

            DrawRectangleLinesEx(
                item.bounds,
                BUTTON_BORDER_THICKNESS,
                BLACK
            );

            DrawText(
                item.name.c_str(),
                static_cast<int>(item.bounds.x + 10),
                static_cast<int>(item.bounds.y + 10),
                TITLE_FONT_SIZE,
                WHITE
            );
        }
    }

} // namespace Interface::UI::BuildingMenuRenderer