#ifndef DAE_LEVEL
#define DAE_LEVEL

#include "Component.h"
#include "Transform.h"
#include <vector>
#include "TextureComponent.h"

namespace dae
{
	class LevelComponent final : public Component
	{
	public:
		LevelComponent(GameObject* ownerObject);

		virtual ~LevelComponent() = default;
		LevelComponent(const LevelComponent& other) = delete;
		LevelComponent(LevelComponent&& other) = delete;
		LevelComponent& operator=(const LevelComponent& other) = delete;
		LevelComponent& operator=(LevelComponent&& other) = delete;

		SourceRectangle GetSourceRect(int column, int row) const;

		void CreateLevel();

		void Render() const override;

	private:
		std::vector<std::vector<int>> m_Level;
		glm::vec3 m_BasePosition;

	};
}

#endif