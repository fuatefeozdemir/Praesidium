#pragma once
#include "../../../Data/CoreData/AppState.h"

namespace Interface::UI::CoreUI {
    namespace MainMenuSystem {
        void Initialize();
        void Update(AppState& currentState);
        void Draw();
    }
}