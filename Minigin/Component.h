#ifndef COMPONENT
#define COMPONENT

namespace dae
{
	class GameObject;

	class Component
	{
	public:
		virtual void Update(float const deltaTime);
		virtual void Render() const;

		void SetShouldRemove();
		bool GetSouldRemove() const { return m_ShouldRemove; }

		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator= (const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

	private:
		GameObject* m_pOwnerObject{};
	protected:
		Component(GameObject* ownerObject);
		GameObject* GetOwner() const;
		bool m_ShouldRemove{ false };
	};
}

#endif