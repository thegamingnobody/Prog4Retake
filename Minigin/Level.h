#ifndef DAE_LEVEL
#define DAE_LEVEL

#include "Component.h"
#include "Transform.h"
#include <vector>
#include "TextureComponent.h"
#include "Observer.h"

namespace dae
{
	struct TileCoordinates
	{
		TileCoordinates(int column, int row) 
			: m_Column	(column) 
			, m_Row		(row)
		{}

		int m_Column;
		int m_Row;
	};

	struct TileData
	{
		TileData(float tileSide, float zoomLevel)
			: m_TileSide(tileSide)
			, m_ZoomLevel(zoomLevel)
		{}

		float m_TileSide;
		float m_ZoomLevel;
	};

	struct LevelData
	{
		LevelData(int round = 1, int tileset = 1, int maxToggles = 1, bool allowTileDecrease = false)
			: m_IsValid(true)
			, m_Round(round)
			, m_TileSet(tileset)
			, m_MaxToggles(maxToggles)
			, m_AllowTileDecrease(allowTileDecrease)
		{}
		
		bool m_IsValid;
		int m_Round;
		int m_TileSet;
		int m_MaxToggles;
		bool m_AllowTileDecrease;
	};

	class LevelComponent final : public Component, public Observer
	{
	public:
		LevelComponent(GameObject* ownerObject, TileData tileData, LevelData levelData);

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

		void ToggleTile(int column, int row);

	private:
		std::vector<std::vector<int>> m_Level;
		glm::vec3 m_BasePosition;
		float const m_TileSide;
		float const m_ZoomLevel;

		LevelData m_LevelData;
		//int m_TileSet;
		//int const m_MaxTileToggles;

	};
}

#endif