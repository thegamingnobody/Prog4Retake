#ifndef QBERTCOMPONENT
#define QBERTCOMPONENT

//#include "Level.h"
#include "Observer.h"
#include "Component.h"
#include "QBertStates.h"

namespace dae
{
    class QbertComponent : public Component, public Observer
    {
    public:
        QbertComponent(GameObject* object, GameObject* curseObject, int startColumn = 0, int startRow = 0);

        void Update(float const deltaTime) override;

        virtual ~QbertComponent();
        QbertComponent(const QbertComponent& other) = delete;
        QbertComponent(QbertComponent&& other) = delete;
        QbertComponent& operator=(const QbertComponent& other) = delete;
        QbertComponent& operator=(QbertComponent&& other) = delete;

        void Notify(const Event& event) override;

        void SetState(std::unique_ptr<State>&& newState, bool callOnExit = true);

        TileCoordinates GetCoords() const { return m_Coordinates; }

    private:
        TileCoordinates m_Coordinates;

        GameObject* m_CurseObject;

        std::unique_ptr<State> m_PlayerState;
    };
}

#endif