#include "../../../include/Engine/Core/Application.h"
#include "../../../include/Engine/Core/GameStates.h"
#include "raylib.h"
#include "../../../include/Engine/Core/Window.h"
#include "../../../include/Engine/Core/Camera.h"
#include "../../../include/Interface/UI/MainMenuRenderer.h"
#include "../../../include/Systems/MapSystem.h"

namespace Engine::Core::Application {

    // --- BAŞLATMA FONKSİYONLARI ---
    namespace {
        void InitializeEngine() {
            Window::Initialize();
            Camera::Initialize();
        }

        void InitializeGame(Data::CoreData::GameContext& context) {
            Interface::UI::MainMenuRenderer::Initialize();
            context.player = {"Oyuncu", {0.0f, 0.0f}, 300.0f, 15.0f, Data::EntityData::PlayerActionState::IDLE, 100.0f, 100.0f, 20, {}};
            Systems::MapSystem::Initialize(context.worldMap, 100, 100, 64);
        }
    }

    void Run() {
        InitializeEngine();

        Data::CoreData::GameContext gameContext;
        InitializeGame(gameContext);

        // Durum nesnelerini (State Objects) oluştur
        MainMenuState mainMenu;
        ActiveSimulationState activeSim;
        PausedState pausedSim;

        // Aktif durum işaretçisi (Pointer)
        IGameState* activeState = &mainMenu;

        // --- ANA OYUN DÖNGÜSÜ ---
        while (!WindowShouldClose() && gameContext.currentState != AppState::EXIT_REQUESTED) {
            if (IsKeyPressed(KEY_F11)) ToggleFullscreen();

            // Durum değişmişse işaretçiyi (pointer) ilgili sınıfa yönlendir
            switch (gameContext.currentState) {
                case AppState::MAIN_MENU: activeState = &mainMenu; break;
                case AppState::ACTIVE_SIMULATION: activeState = &activeSim; break;
                case AppState::PAUSED: activeState = &pausedSim; break;
                default: break;
            }

            // GÜNCELLEME (Aktif sınıf kendi Update fonksiyonunu çalıştırır)
            activeState->Update(&gameContext);

            // ÇİZİM (Aktif sınıf kendi Draw fonksiyonunu çalıştırır)
            BeginDrawing();
            activeState->Draw(&gameContext);
            EndDrawing();
        }

        Interface::UI::MainMenuRenderer::Close();
        Window::Close();
    }
}