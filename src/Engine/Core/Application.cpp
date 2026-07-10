#include "../../../include/Engine/Core/Application.h"

#include "../../../include/Engine/Core/GameStates.h"
#include "../../../include/Engine/Core/Window.h"

#include "../../../include/Interface/UI/MainMenuRenderer.h"

#include "../../../include/Systems/CameraSystem.h"
#include "../../../include/Systems/MapSystem.h"
#include "../../../include/Systems/ChunkSystem.h"

#include "raylib.h"

namespace Engine::Core::Application {

    namespace {

        constexpr uint64_t WORLD_SEED = 12345;
        constexpr int WORLD_WIDTH = 100;
        constexpr int WORLD_HEIGHT = 100;

        constexpr double FIXED_TIME_STEP = 1.0 / 60.0;

        void InitializeEngine() {

            Window::Initialize();

            Systems::CameraSystem::Initialize();
            Systems::ChunkSystem::Initialize();
        }

        void InitializeGame(Data::CoreData::GameContext& context) {

            Interface::UI::MainMenuRenderer::Initialize();

            context.player = {
                "Oyuncu",
                {5000, 5000},
                300,
                16,
                Data::EntityData::PlayerActionState::IDLE,
                100,
                100,
                20,
                {}
            };

            Systems::MapSystem::Initialize(
                context.worldMap,
                WORLD_SEED,
                WORLD_WIDTH,
                WORLD_HEIGHT
            );
        }

    } // namespace

    void Run() {

        InitializeEngine();

        auto* gameContext = new Data::CoreData::GameContext();
        InitializeGame(*gameContext);

        MainMenuState mainMenu;
        ActiveSimulationState activeSimulation;
        PausedState pausedSimulation;

        IGameState* activeState = &mainMenu;

        double accumulator = 0.0;

        while (!WindowShouldClose() &&
               gameContext->currentState != Data::CoreData::AppState::EXIT_REQUESTED) {

            if (IsKeyPressed(KEY_F11)) {
                ToggleFullscreen();
            }

            switch (gameContext->currentState) {

                case Data::CoreData::AppState::MAIN_MENU:
                    activeState = &mainMenu;
                    break;

                case Data::CoreData::AppState::ACTIVE_SIMULATION:
                    activeState = &activeSimulation;
                    break;

                case Data::CoreData::AppState::PAUSED:
                    activeState = &pausedSimulation;
                    break;

                default:
                    break;
            }

            accumulator += GetFrameTime();

            while (accumulator >= FIXED_TIME_STEP) {
                activeState->Update(gameContext);
                accumulator -= FIXED_TIME_STEP;
            }

            BeginDrawing();
            activeState->Draw(gameContext);
            EndDrawing();
        }

        Interface::UI::MainMenuRenderer::Close();

        delete gameContext;

        Window::Close();
    }

} // namespace Engine::Core::Application