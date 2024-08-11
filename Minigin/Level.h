#ifndef DAE_LEVEL
#define DAE_LEVEL

#include "Component.h"
#include "Transform.h"
#include <vector>
#include "TextureComponent.h"
#include "Observer.h"
#include "../QBert/QbertStructs.h"


namespace dae
{
	class LevelComponent final : public Component, public Observer
	{
	public:
		LevelComponent(GameObject* ownerObject, TileData tileData, RoundData levelData);

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

		void LoadNewRound();

	private:
		bool IsLevelFinished();
		
		std::vector<std::vector<int>> m_Level;
		glm::vec3 m_BasePosition;
		float const m_TileSide;
		float const m_ZoomLevel;

		RoundData m_LevelData;
		//int m_TileSet;
		//int const m_MaxTileToggles;

	};
}

#endif