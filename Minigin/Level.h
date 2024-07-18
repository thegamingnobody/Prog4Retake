#ifndef DAE_LEVEL
#define DAE_LEVEL

#include "Component.h"
#include "Transform.h"
#include <vector>
#include "TextureComponent.h"
#include <Observer.h>

namespace dae
{
	struct TileCoordinates
	{
		TileCoordinates(int column, int row) 
		{
			m_Column = column; 
			m_Row = row; 
		}
		int m_Column;
		int m_Row;
	};
	class LevelComponent final : public Component, public Observer
	{
	public:
		LevelComponent(GameObject* ownerObject, float tileSide, float zoomLevel);

		virtual ~LevelComponent() = default;
		LevelComponent(const LevelComponent& other) = delete;
		LevelComponent(LevelComponent&& other) = delete;
		LevelComponent& operator=(const LevelComponent& other) = delete;
		LevelComponent& operator=(LevelComponent&& other) = delete;

		SourceRectangle GetSourceRect(int column, int row) const;

		void CreateLevel();

		void Render() const override;

		bool DoesTileExist(int column, int row);

		void Notify(const Event& event) override;

		glm::vec3 ConvertToWorld(int columnOffset, int rowOffset);

		float GetTileSide() const { return m_TileSide; }

	private:
		std::vector<std::vector<int>> m_Level;
		glm::vec3 m_BasePosition;
		float const m_TileSide;
		float const m_ZoomLevel;

	};
}

#endif