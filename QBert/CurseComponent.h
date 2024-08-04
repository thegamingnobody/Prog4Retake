#pragma once
#include <Component.h>
#include <Observer.h>

namespace dae
{
	class CurseComponent : public Component, public Observer
	{
	public:
        CurseComponent(GameObject* object);

        virtual ~CurseComponent() = default;
        CurseComponent(const CurseComponent& other) = delete;
        CurseComponent(CurseComponent&& other) = delete;
        CurseComponent& operator=(const CurseComponent& other) = delete;
        CurseComponent& operator=(CurseComponent&& other) = delete;

        void Notify(const Event& event) override;

	private:
	};
}