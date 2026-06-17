#pragma once
#include "raylib.h"
#include <string>

namespace UI {
    enum class ButtonState { NORMAL, HOVERED, PRESSED };

    // Ekranda hizalama noktaları
    enum class Alignment { CENTER, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };

    // Renk paleti
    struct Theme {
        Color background;
        Color normal;
        Color hovered;
        Color pressed;
        Color textNormal;
        Color textHovered;
        Color border;
    };

    struct Button {
        Rectangle bounds;       // Anlık hesaplanan X, Y, Genişlik, Yükseklik
        float width, height;    // İstenen sabit genişlik ve yükseklik
        float offsetX, offsetY; // Hizalama noktasına göre X ve Y sapması
        Alignment align;        // Hizalama tipi
        std::string text;
        ButtonState state;
    };
}