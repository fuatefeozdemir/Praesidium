#pragma once

namespace Engine::Core::Window {

    // Default window settings
    constexpr int STARTUP_WIDTH = 1280;
    constexpr int STARTUP_HEIGHT = 720;
    constexpr const char* WINDOW_TITLE = "Praesidium";
    constexpr int TARGET_FPS = 60;

    void Initialize();
    void Close();

    // TODO: Move additional window-related constants here
    // (fullscreen, minimum size, icon, etc.).

} // namespace Engine::Core::Window