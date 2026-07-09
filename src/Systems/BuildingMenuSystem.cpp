#include "../../include/Systems/BuildingMenuSystem.h"

#include "raylib.h"

namespace {

    constexpr float PANEL_PADDING = 20.0f;
    constexpr float TAB_WIDTH = 80.0f;
    constexpr float TAB_HEIGHT = 30.0f;

    constexpr float ITEM_WIDTH = 200.0f;
    constexpr float ITEM_HEIGHT = 40.0f;
    constexpr float ITEM_OFFSET_Y = 60.0f;

    constexpr float TOGGLE_BUTTON_MARGIN = 20.0f;

}

namespace Systems::BuildingMenuSystem {

    void LoadCategoryItems(Data::UIData::BuildingMenuState* state) {

        state->currentCategoryItems.clear();

        switch (state->activeCategory) {

            case Data::UIData::BuildingCategory::LOGISTICS: {

                Data::UIData::BuildingMenuItem conveyor{};

                conveyor.type = Data::WorldData::BuildingType::CONVEYOR_BELT;
                conveyor.name = "Konveyor";
                conveyor.description = "Esyalari tasir.";

                conveyor.cost[Data::WorldData::ItemType::IRON_ORE] = 2;

                conveyor.bounds = {
                    state->panelBounds.x + PANEL_PADDING,
                    state->panelBounds.y + ITEM_OFFSET_Y,
                    ITEM_WIDTH,
                    ITEM_HEIGHT
                };

                state->currentCategoryItems.push_back(conveyor);

                break;
            }

            default:
                // TODO: Load items for the remaining building categories.
                break;
        }
    }

    bool Update(Data::UIData::BuildingMenuState* state) {

        const Vector2 mousePosition = GetMousePosition();

        state->toggleButtonBounds.x =
            static_cast<float>(GetScreenWidth()) -
            state->toggleButtonBounds.width -
            TOGGLE_BUTTON_MARGIN;

        state->toggleButtonBounds.y =
            static_cast<float>(GetScreenHeight()) -
            state->toggleButtonBounds.height -
            TOGGLE_BUTTON_MARGIN;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(mousePosition, state->toggleButtonBounds)) {

            state->isOpen = !state->isOpen;

            if (state->isOpen) {
                LoadCategoryItems(state);
            }

            return true;
        }

        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            state->selectedBuilding = Data::WorldData::BuildingType::NONE;
        }

        if (!state->isOpen) {
            return false;
        }

        if (!CheckCollisionPointRec(mousePosition, state->panelBounds)) {
            return false;
        }

        if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return true;
        }

        const Rectangle logisticsTab = {
            state->panelBounds.x + PANEL_PADDING,
            state->panelBounds.y + 10.0f,
            TAB_WIDTH,
            TAB_HEIGHT
        };

        if (CheckCollisionPointRec(mousePosition, logisticsTab) &&
            state->activeCategory != Data::UIData::BuildingCategory::LOGISTICS) {

            state->activeCategory = Data::UIData::BuildingCategory::LOGISTICS;
            LoadCategoryItems(state);
        }

        for (const auto& item : state->currentCategoryItems) {

            if (!CheckCollisionPointRec(mousePosition, item.bounds)) {
                continue;
            }

            state->selectedBuilding = item.type;
            break;
        }

        return true;
    }

} // namespace Systems::BuildingMenuSystem