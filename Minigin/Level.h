#ifndef DAE_LEVEL
#define DAE_LEVEL

#include "Component.h"
#include "Transform.h"
#include <vector>
#include "TextureComponent.h"
#include "Observer.h"
#include "../QBert/QbertStructs.h"
#include "../QBert/QbertComponent.h"


namespace dae
{
	class QbertComponent;

	class LevelComponent final : public Component, public Observer
	{
	public:
		LevelComponent(GameObject* ownerObject, TileData tileData, RoundData levelData, QbertComponent& playerComponent);

		virtual ~LevelComponent();
		LevelComponent(const LevelComponent& other) = delete;
		LevelComponent(LevelComponent&& other) = delete;
		LevelComponent& operator=(const LevelComponent& other) = delete;
		LevelComponent& operator=(LevelComponent&& other) = delete;

		SourceRectangle GetSourceRect(int column, int row) const;

		void CreateLevel();

		void Update(float const deltaTime) override;
		void Render() const override;

		bool DoesTileExist(int column, int row);

		void Notify(const Event& event) override;

		glm::vec3 ConvertToWorld(int columnOffset, int rowOffset);

		float GetTileSide() const { return m_TileSide; }

		void ToggleTile(int column, int row);

		void LoadNewRound();

		void SpawnCoily();

	private:
		bool IsLevelFinished();

		void PlayLevelDoneAnim(bool playAnim);
		void HandleLevelFlashing();

		std::vector<std::vector<int>> m_Level;
		glm::vec3 m_BasePosition;
		float const m_TileSide;
		float const m_ZoomLevel;

		RoundData m_LevelData;

		bool m_PlayLevelDoneAnim;
		float m_AcumulatedTime;
		float m_FlashCycleTime;
		int m_FlashCyclesCountdown;

		int m_NextEnemyID;
		QbertComponent& m_PlayerComponent;
		float m_CoilySpawnTimer;
		bool m_CanSpawnCoily;
	};
}

#endif