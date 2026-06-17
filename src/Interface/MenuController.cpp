#include "../../include/Interface/MenuController.h"
#include "raylib.h"

namespace Interface {
    void UpdateMainMenu(std::vector<UI::Button>& buttons, AppState& currentState) {
        Vector2 mousePos = GetMousePosition();

        for (auto& btn : buttons) {
            // İmleç dikdörtgen sınırlarında mı
            if (CheckCollisionPointRec(mousePos, btn.bounds)) {
                
                // Sol tık basılı tutuluyorsa
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    btn.state = UI::ButtonState::PRESSED;
                }

                // Sol tık bırakıldıysa (Tıklandı)
                else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    if (btn.text == "OYUNA BASLA") currentState = AppState::ACTIVE_SIMULATION;
                    else if (btn.text == "CIKIS") currentState = AppState::EXIT_REQUESTED;
                    
                    btn.state = UI::ButtonState::HOVERED;
                }

                // İmleç sadece üzerinde duruyorsa
                else {
                    btn.state = UI::ButtonState::HOVERED;
                }
                
            } else {
                // Fare butonun üzerinde değilse
                btn.state = UI::ButtonState::NORMAL;
            }
        }
    }
}