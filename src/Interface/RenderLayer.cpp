#include "../../include/Interface/RenderLayer.h"
#include "raylib.h"

namespace Interface {
    void DrawMainMenu(const std::vector<UI::Button>& buttons) {
        ClearBackground(RAYWHITE);

        // Başlık
        DrawText("PRAESIDIUM", 50, 50, 40, DARKBLUE);

        // Butonları Çiz
        for (const auto& btn : buttons) {
            Color bgColor;
            Color textColor = WHITE;

            // Duruma göre renk seçimi
            switch (btn.state) {
                case UI::ButtonState::NORMAL:
                    bgColor = DARKGRAY;
                break;
                case UI::ButtonState::HOVERED:
                    bgColor = LIGHTGRAY;
                textColor = BLACK;
                break;
                case UI::ButtonState::PRESSED:
                    bgColor = BLACK;
                break;
            }

            // Arka plan
            DrawRectangleRec(btn.bounds, bgColor);

            // Çerçeve (Border)
            DrawRectangleLinesEx(btn.bounds, 2.0f, BLACK);

            // Metni butonun ortasına hizala
            int textWidth = MeasureText(btn.text.c_str(), 20);
            int textX = btn.bounds.x + (btn.bounds.width / 2) - (textWidth / 2);
            int textY = btn.bounds.y + (btn.bounds.height / 2) - 10;

            DrawText(btn.text.c_str(), textX, textY, 20, textColor);
        }
    }
}