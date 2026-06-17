#pragma once
#include "UIData.h"
#include "../Engine/AppState.h"
#include <vector>

namespace Interface {
    void UpdateMainMenu(std::vector<UI::Button>& buttons, AppState& currentState);
}