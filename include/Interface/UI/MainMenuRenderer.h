#pragma once
#include "../../Data/CoreData/AppState.h"

namespace Interface::UI::MainMenuRenderer {
    void Initialize();
    void Update(AppState& currentState);
    void Draw();
    void Close();
}