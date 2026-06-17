#pragma once

namespace Config {
    // Varsayılan / Güvenli Pencere Ayarları
    constexpr int DEFAULT_WINDOW_WIDTH = 1280;
    constexpr int DEFAULT_WINDOW_HEIGHT = 720;
    constexpr const char* WINDOW_TITLE = "Praesidium";
    constexpr int TARGET_FPS = 60;

    // Harita ve Grid Sabitleri
    constexpr int TILE_SIZE = 32;
    constexpr int MAP_WIDTH = 50;
    constexpr int MAP_HEIGHT = 50;
}