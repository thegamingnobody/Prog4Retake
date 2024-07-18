#pragma once
#include <Component.h>
#include <Observer.h>
#include "Level.h"

namespace dae
{
    class QbertComponent : public Component, public Observer
    {
    public:
        enum class MovementState
        {
            Idle,
            Requesting,
            Moving
        };

        QbertComponent(GameObject* object, int startColumn = 0, int startRow = 0);

        void Update(float const deltaTime) override;

        virtual ~QbertComponent() = default;
        QbertComponent(const QbertComponent& other) = delete;
        QbertComponent(QbertComponent&& other) = delete;
        QbertComponent& operator=(const QbertComponent& other) = delete;
        QbertComponent& operator=(QbertComponent&& other) = delete;

        void Notify(const Event& event) override;

        

    private:
        TileCoordinates m_Coordinates;
        TileCoordinates m_TilesDirection;
        bool m_DirectionSet;
        MovementState m_MovementState;
    };
}