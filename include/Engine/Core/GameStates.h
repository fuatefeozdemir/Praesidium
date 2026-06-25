#pragma once
#include "../../Data/CoreData/GameContext.h"

namespace Engine::Core {

    // Ortak Arayüz (Interface)
    class IGameState {
    public:
        virtual ~IGameState() = default;
        virtual void Update(Data::CoreData::GameContext* context) = 0;
        virtual void Draw(Data::CoreData::GameContext* context) = 0;
    };

    // Ana Menü Durumu
    class MainMenuState : public IGameState {
    public:
        void Update(Data::CoreData::GameContext* context) override;
        void Draw(Data::CoreData::GameContext* context) override;
    };

    // Simülasyon Durumu
    class ActiveSimulationState : public IGameState {
    public:
        void Update(Data::CoreData::GameContext* context) override;
        void Draw(Data::CoreData::GameContext* context) override;
    };

    // Duraklatma Durumu
    class PausedState : public IGameState {
    public:
        void Update(Data::CoreData::GameContext* context) override;
        void Draw(Data::CoreData::GameContext* context) override;
    };
}