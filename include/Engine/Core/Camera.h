#pragma once
#include "raylib.h"

namespace Engine::Core::Camera {
    void Initialize();
    void Update(int targetX, int targetY);    void BeginWorldDrawing();
    void EndWorldDrawing();
    Vector2 GetWorldMousePosition();
}