#pragma once

#include <string>

#include "raylib.h"

namespace Data::UIData {

    enum class ButtonState {
        NORMAL,
        HOVERED,
        PRESSED
    };

    enum class Alignment {
        CENTER,
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT
    };

    struct Theme {
        // Color palette
        Color background{};
        Color normal{};
        Color hovered{};
        Color pressed{};
        Color textNormal{};
        Color textHovered{};
        Color border{};
    };

    struct Button {
        // Calculated bounds
        Rectangle bounds = {0.0f, 0.0f, 0.0f, 0.0f};

        // Fixed size
        float width = 0.0f;
        float height = 0.0f;

        // Alignment offset
        float offsetX = 0.0f;
        float offsetY = 0.0f;

        // Alignment point
        Alignment align = Alignment::CENTER;

        std::string text;
        ButtonState state = ButtonState::NORMAL;

        // TODO: Separate button layout data from runtime state
        // if the UI system becomes more complex.
    };

} // namespace Data::UIData