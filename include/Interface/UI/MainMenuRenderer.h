#pragma once

#include "../../Data/CoreData/AppState.h"

namespace Interface::UI::MainMenuRenderer {

    // Loads textures, fonts, and UI layout.
    void Initialize();

    // Processes menu input and state transitions.
    void Update(Data::CoreData::AppState& currentState);

    // Renders the main menu.
    void Draw();

    // Releases loaded resources.
    void Close();

} // namespace Interface::UI::MainMenuRenderer