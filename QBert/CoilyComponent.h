#pragma once
#include <Component.h>
#include <Observer.h>
#include "Level.h"
#include "CoilyStates.h"
#include "QbertComponent.h"

namespace dae
{
    class CoilyComponent : public Component, public Observer
    {
    public:
        enum class CoilyForm
        {
            Egg = 2,
            Snake = 4
        };
        
        CoilyComponent(GameObject* object, dae::QbertComponent& playerComponent, int startColumn = 0, int startRow = 0);

        void Update(float const deltaTime) override;

        virtual ~CoilyComponent();
        CoilyComponent(const CoilyComponent& other) = delete;
        CoilyComponent(CoilyComponent&& other) = delete;
        CoilyComponent& operator=(const CoilyComponent& other) = delete;
        CoilyComponent& operator=(CoilyComponent&& other) = delete;

        void Notify(const Event& event) override;

        void SetState(std::unique_ptr<State>&& newState, bool callOnExit = true);

        TileCoordinates GetCoords() const { return m_Coordinates; }
        TileCoordinates GetPlayerCoords() const { return m_PlayerCoordinates; }

        CoilyForm GetCoilyForm() { return m_CoilyForm; }
        void HatchIntoCoily();

        

    private:
        TileCoordinates m_Coordinates;

        std::unique_ptr<State> m_CoilyState;

        CoilyForm m_CoilyForm;

        dae::QbertComponent& m_PlayerComponent;
        TileCoordinates m_PlayerCoordinates;
    };
}

