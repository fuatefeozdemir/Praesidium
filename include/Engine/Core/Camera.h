#pragma once
#include "raylib.h"

namespace Engine::Core::Camera {
    void Initialize();
    void Update(Vector2 targetPosition);
    void BeginWorldDrawing();
    void EndWorldDrawing();
    Vector2 GetWorldMousePosition();
}