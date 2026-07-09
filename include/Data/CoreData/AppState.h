#pragma once

namespace Data::CoreData {

    // Represents the current state of the application.
    enum class AppState {
        SPLASH_SCREEN,      // Initial loading screen
        MAIN_MENU,          // Main menu
        SAVE_LOAD_MENU,     // Save and load game menu
        SETTINGS,           // Settings menu
        ACTIVE_SIMULATION,  // Main gameplay
        PAUSED,             // Pause menu
        GAME_OVER,          // Game over screen
        EXIT_REQUESTED      // Close the application
    };

}// namespace Data::CoreData
