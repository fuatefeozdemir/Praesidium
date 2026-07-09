#include "../../../include/Engine/Core/Application.h"
#include "../../../include/Engine/Core/GameStates.h"
#include "raylib.h"
#include "../../../include/Engine/Core/Window.h"
#include "../../../include/Systems/CameraSystem.h"
#include "../../../include/Interface/UI/MainMenuRenderer.h"
#include "../../../include/Systems/MapSystem.h"

namespace Engine::Core::Application {

    // --- BAŞLATMA FONKSİYONLARI ---
    namespace {
        void InitializeEngine() {
            Window::Initialize();
            Systems::CameraSystem::Initialize();
        }

        void InitializeGame(Data::CoreData::GameContext& context) {
            Interface::UI::MainMenuRenderer::Initialize();
            context.player = {"Oyuncu", {0, 0}, 300, 16, Data::EntityData::PlayerActionState::IDLE, 100, 100, 20, {}};

            // Seed değeri 12345 olarak belirlendi, testler için 100x100 Chunk'lık (3200x3200 Tile) bir dünya sınırı kondu.
            Systems::MapSystem::Initialize(context.worldMap, 12345, 100, 100);
        }
    }

    void Run() {
        InitializeEngine();

        auto* gameContext = new Data::CoreData::GameContext();
        InitializeGame(*gameContext);

        // Durum nesnelerini (State Objects) oluştur
        MainMenuState mainMenu;
        ActiveSimulationState activeSim;
        PausedState pausedSim;

        // Aktif durum işaretçisi (Pointer)
        IGameState* activeState = &mainMenu;

        const double TICK_RATE = 1.0 / 60.0;
        double accumulator = 0.0;

        // --- ANA OYUN DÖNGÜSÜ ---
        while (!WindowShouldClose() && gameContext->currentState != AppState::EXIT_REQUESTED) {
            if (IsKeyPressed(KEY_F11)) ToggleFullscreen();

            // Durum değişmişse işaretçiyi (pointer) ilgili sınıfa yönlendir
            switch (gameContext->currentState) {
                case AppState::MAIN_MENU: activeState = &mainMenu; break;
                case AppState::ACTIVE_SIMULATION: activeState = &activeSim; break;
                case AppState::PAUSED: activeState = &pausedSim; break;
                default: break;
            }

            accumulator += GetFrameTime();

            while (accumulator >= TICK_RATE) {
                // GÜNCELLEME (Aktif sınıf kendi Update fonksiyonunu çalıştırır)
                activeState->Update(gameContext);
                accumulator -= TICK_RATE;
            }

            // ÇİZİM (Aktif sınıf kendi Draw fonksiyonunu çalıştırır)
            BeginDrawing();
            activeState->Draw(gameContext);
            EndDrawing();
        }

        Interface::UI::MainMenuRenderer::Close();
        Window::Close();
    }
}