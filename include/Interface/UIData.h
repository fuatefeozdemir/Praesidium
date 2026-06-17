#pragma once
#include "raylib.h"
#include <string>

namespace UI {
    // Buton durumlarını takip etmek için
    enum class ButtonState {
        NORMAL,
        HOVERED,
        PRESSED
    };

    struct Button {
        Rectangle bounds;       // X, Y, Genişlik, Yükseklik
        std::string text;       // Butonun üzerindeki yazı
        ButtonState state;      // Butonun anlık durumu
    };
}