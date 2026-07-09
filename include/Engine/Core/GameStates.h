#pragma once
#include "../../Data/CoreData/GameContext.h"

namespace Engine::Core {

    // Base game state interface
    class IGameState {
    public:
        virtual ~IGameState() = default;

        virtual void Update(Data::CoreData::GameContext* context) = 0;
        virtual void Draw(Data::CoreData::GameContext* context) = 0;
    };

    // Main menu state
    class MainMenuState : public IGameState {
    public:
        void Update(Data::CoreData::GameContext* context) override;
        void Draw(Data::CoreData::GameContext* context) override;
    };

    // Active simulation state
    class ActiveSimulationState : public IGameState {
    public:
        void Update(Data::CoreData::GameContext* context) override;
        void Draw(Data::CoreData::GameContext* context) override;
    };

    // Pause menu state
    class PausedState : public IGameState {
    public:
        void Update(Data::CoreData::GameContext* context) override;
        void Draw(Data::CoreData::GameContext* context) override;
    };

} // namespace Engine::Core