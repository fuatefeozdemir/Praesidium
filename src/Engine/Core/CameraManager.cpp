#include "../../../include/Engine/Core/CameraManager.h"
#include "raymath.h"

namespace {
    Camera2D camera = { 0 };
}

namespace Engine::Core::CameraManager {

    void Initialize() {
        camera.target = { 0.0f, 0.0f };
        camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
    }

    void Update(Vector2 targetPosition) {
        camera.target = targetPosition;
        float wheelMove = GetMouseWheelMove();
        if (wheelMove != 0.0f) {
            camera.zoom += wheelMove * 0.1f;

            // Zoom sınırları
            if (camera.zoom < 0.2f) camera.zoom = 0.2f;
            if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        }
    }

    void BeginWorldDrawing() {
        BeginMode2D(camera);
    }

    void EndWorldDrawing() {
        EndMode2D();
    }

    Vector2 GetWorldMousePosition() {
        return GetScreenToWorld2D(GetMousePosition(), camera);
    }
}