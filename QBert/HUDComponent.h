#pragma once
#include <Component.h>
#include <Event.h>
#include <Observer.h>
#include <glm/glm.hpp>

namespace dae
{
    enum class HUDType
    {
        Lives,
        GameOver,
    };

    class HUDComponent : public Component, public Observer
    {
    public:

        HUDComponent(dae::GameObject* object, dae::HUDType HUDType);

        void Render() const override;

        void Notify(const Event& event) override;

        void Reset();

        void SubscribeToEvents();
    private:
        dae::HUDType m_HUDType;

        int const m_StartingHealth;
        int m_CurrentHealth;
        glm::vec3 m_HealthStartPos;
    };
}

