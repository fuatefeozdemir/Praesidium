#include "../../include/Systems/CameraSystem.h"
#include "raymath.h"

namespace {
    // Şimdilik global, ileride GameContext veya WindowContext içine taşınabilir
    Camera2D camera{};
}

namespace Systems::CameraSystem {

    void Initialize() {
        camera.target = { 0.0f, 0.0f };
        camera.offset = { static_cast<float>(GetScreenWidth()) / 2.0f, static_cast<float>(GetScreenHeight()) / 2.0f };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
    }

    void Update() {
        // İleride input sistemi ayrıldığında bu blok oraya taşınabilir,
        // şimdilik en temel kamera özelliği olduğu için burada kalması pratik.
        float wheelMove = GetMouseWheelMove();
        if (wheelMove != 0.0f) {
            float newZoom = camera.zoom + (wheelMove * 0.1f);
            SetZoom(newZoom);
        }
    }

    void BeginWorldDrawing() {
        BeginMode2D(camera);
    }

    void EndWorldDrawing() {
        EndMode2D();
    }

    void SetTarget(Vector2 target) {
        camera.target = target;
    }

    void SetZoom(float zoom) {
        camera.zoom = zoom;

        // Zoom sınırları (Min: 0.2x, Max: 3.0x)
        if (camera.zoom < 0.2f) camera.zoom = 0.2f;
        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
    }

    void Move(Vector2 delta) {
        camera.target = Vector2Add(camera.target, delta);
    }

    Vector2 GetWorldMousePosition() {
        return ScreenToWorld(GetMousePosition());
    }

    Vector2 ScreenToWorld(Vector2 position) {
        return GetScreenToWorld2D(position, camera);
    }

    Vector2 WorldToScreen(Vector2 position) {
        return GetWorldToScreen2D(position, camera);
    }

    const Camera2D& GetCamera() {
        return camera;
    }
}