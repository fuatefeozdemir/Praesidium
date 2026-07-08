#pragma once
#include "raylib.h"

namespace Systems::CameraSystem {

    // Başlangıç ayarlarını yapar
    void Initialize();

    // Her frame çağrılır (Zoom mantığını yönetir)
    void Update();

    // Çizim modlarını başlatır ve bitirir
    void BeginWorldDrawing();
    void EndWorldDrawing();

    // Temel Kamera Manipülasyonları
    void SetTarget(Vector2 target);
    void SetZoom(float zoom);
    void Move(Vector2 delta);

    // Koordinat Dönüşümleri ve Bilgi
    Vector2 GetWorldMousePosition();
    Vector2 ScreenToWorld(Vector2 position);
    Vector2 WorldToScreen(Vector2 position);

    // Raylib fonksiyonları için kamera verisine doğrudan referans erişimi
    const Camera2D& GetCamera();

}