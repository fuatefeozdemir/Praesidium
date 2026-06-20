#pragma once
#include "raylib.h"
#include <string>

namespace UI {

    class FloatingText {
    private:
        Vector2 position;
        std::string text;
        Color color;
        float alpha;
        float lifetime;
        float maxLifetime;
        float speedY;

    public:
        FloatingText(Vector2 pos, const std::string& txt, Color col, float duration = 1.0f)
            : position(pos), text(txt), color(col), alpha(1.0f), lifetime(0.0f), maxLifetime(duration), speedY(-50.0f) {}

        void Update(float dt) {
            lifetime += dt;
            position.y += speedY * dt;                  // Metni yukarı doğru kaydır
            alpha = 1.0f - (lifetime / maxLifetime);   // Zamanla şeffaflaştır (fade out)
        }

        void Draw() const {
            if (lifetime < maxLifetime) {
                DrawText(text.c_str(), (int)position.x, (int)position.y, 20, Fade(color, alpha));
            }
        }

        bool IsDead() const { 
            return lifetime >= maxLifetime; 
        }
    };
}