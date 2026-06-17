#include "../../../../include/Interface/UI/CoreUI/MainMenuSystem.h"
#include "../../../../include/Data/UIData/UIData.h"
#include "raylib.h"
#include <vector>

namespace {
    // Sadece bu dosyada yaşayan gizli (private) veriler
    std::vector<::UI::Button> buttons;
    ::UI::Theme theme;

    // Layout hesaplamasını yapan dahili yardımcı fonksiyon
    void UpdateLayout() {
        float screenW = (float)GetScreenWidth();
        float screenH = (float)GetScreenHeight();

        for (auto& btn : buttons) {
            btn.bounds.width = btn.width;
            btn.bounds.height = btn.height;

            if (btn.align == ::UI::Alignment::CENTER) {
                btn.bounds.x = (screenW / 2.0f) - (btn.width / 2.0f) + btn.offsetX;
                btn.bounds.y = (screenH / 2.0f) - (btn.height / 2.0f) + btn.offsetY;
            }
        }
    }
}

namespace Interface::UI::CoreUI {
    namespace MainMenuSystem {
        
        void Initialize() {
            // Tema ve buton verilerini başlat
            theme = { RAYWHITE, DARKGRAY, LIGHTGRAY, BLACK, WHITE, BLACK, BLACK };

            buttons = {
                {{0,0,0,0}, 200, 50, 0, -60, ::UI::Alignment::CENTER, "OYUNA BASLA", ::UI::ButtonState::NORMAL},
                {{0,0,0,0}, 200, 50, 0, 20, ::UI::Alignment::CENTER, "CIKIS", ::UI::ButtonState::NORMAL}
            };
        }

        void Update(AppState& currentState) {
            // Önce koordinatları ekrana göre güncelle
            UpdateLayout();

            // Sonra tıklama mantığını çalıştır
            Vector2 mousePos = GetMousePosition();

            for (auto& btn : buttons) {
                if (CheckCollisionPointRec(mousePos, btn.bounds)) {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        btn.state = ::UI::ButtonState::PRESSED;
                    } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                        if (btn.text == "OYUNA BASLA") currentState = AppState::ACTIVE_SIMULATION;
                        else if (btn.text == "CIKIS") currentState = AppState::EXIT_REQUESTED;
                        
                        btn.state = ::UI::ButtonState::HOVERED;
                    } else {
                        btn.state = ::UI::ButtonState::HOVERED;
                    }
                } else {
                    btn.state = ::UI::ButtonState::NORMAL;
                }
            }
        }

        void Draw() {
            ClearBackground(theme.background);
            DrawText("PRAESIDIUM", 50, 50, 40, DARKBLUE);

            for (const auto& btn : buttons) {
                Color bgColor;
                Color textColor;

                switch (btn.state) {
                    case ::UI::ButtonState::NORMAL:  
                        bgColor = theme.normal; 
                        textColor = theme.textNormal;
                        break;
                    case ::UI::ButtonState::HOVERED: 
                        bgColor = theme.hovered; 
                        textColor = theme.textHovered; 
                        break;
                    case ::UI::ButtonState::PRESSED: 
                        bgColor = theme.pressed; 
                        textColor = theme.textNormal;
                        break;
                }

                DrawRectangleRec(btn.bounds, bgColor);
                DrawRectangleLinesEx(btn.bounds, 2.0f, theme.border);

                int textWidth = MeasureText(btn.text.c_str(), 20);
                int textX = btn.bounds.x + (btn.bounds.width / 2) - (textWidth / 2);
                int textY = btn.bounds.y + (btn.bounds.height / 2) - 10;

                DrawText(btn.text.c_str(), textX, textY, 20, textColor);
            }
        }
    }
}