#pragma once
#include "raylib.h"

namespace Systems::CameraSystem {

    // Initialization
    void Initialize();

    // Update
    void Update();

    // Drawing
    void BeginWorldDrawing();
    void EndWorldDrawing();

    // Camera controls
    void SetTarget(Vector2 target);
    void SetZoom(float zoom);
    void Move(Vector2 delta);

    // Coordinate conversion
    Vector2 GetWorldMousePosition();
    Vector2 ScreenToWorld(Vector2 position);
    Vector2 WorldToScreen(Vector2 position);
    Rectangle GetViewBounds();

    // Camera access
    const Camera2D& GetCamera();

} // namespace Systems::CameraSystem