#include "../../include/Systems/CameraSystem.h"

#include "raymath.h"

namespace {

    constexpr float MIN_ZOOM = 0.2f;
    constexpr float MAX_ZOOM = 3.0f;
    constexpr float ZOOM_STEP = 0.1f;

    // Global camera instance
    Camera2D camera{};

} // namespace

namespace Systems::CameraSystem {

    void Initialize() {
        camera.target = {0.0f, 0.0f};
        camera.offset = {
            static_cast<float>(GetScreenWidth()) / 2.0f,
            static_cast<float>(GetScreenHeight()) / 2.0f
        };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
    }

    void Update() {
        float wheelMove = GetMouseWheelMove();

        if (wheelMove != 0.0f) {
            SetZoom(camera.zoom + wheelMove * ZOOM_STEP);
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

        if (camera.zoom < MIN_ZOOM) camera.zoom = MIN_ZOOM;
        if (camera.zoom > MAX_ZOOM) camera.zoom = MAX_ZOOM;
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

    Rectangle GetViewBounds() {
        Vector2 topLeft = ScreenToWorld({0.0f, 0.0f});
        Vector2 bottomRight = ScreenToWorld({
            static_cast<float>(GetScreenWidth()),
            static_cast<float>(GetScreenHeight())
        });

        return {
            topLeft.x,
            topLeft.y,
            bottomRight.x - topLeft.x,
            bottomRight.y - topLeft.y
        };
    }

    const Camera2D& GetCamera() {
        return camera;
    }

} // namespace Systems::CameraSystem