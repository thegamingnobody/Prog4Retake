#include "Level.h"
#include <Renderer.h>
#include <GameObject.h>
#include "TransformComponent.h"
#include <EventManager.h>
#include <glm/ext/vector_float3.hpp>
#include "../QBert/LevelLoader.h"


dae::LevelComponent::LevelComponent(GameObject* ownerObject, TileData tileData, RoundData levelData)
	: Component(ownerObject)
	, m_Level()
	, m_BasePosition()
	, m_TileSide(tileData.m_TileSide)
	, m_ZoomLevel(tileData.m_ZoomLevel)
	, m_LevelData(levelData)
{
	CreateLevel();
	m_TargetNumber = ownerObject->GetObjectID();

	auto transformCpnt = GetOwner()->GetComponent<dae::TransformComponent>();
	if (transformCpnt != nullptr)
	{
		m_BasePosition = transformCpnt->GetPosition().GetPosition();
	}
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::IsTileValid);
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::ToggleTile);
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::LoadNextLevel);
}

dae::SourceRectangle dae::LevelComponent::GetSourceRect(int column, int row) const
{
	assert(m_Level.size() - 1 >= column);
	assert(m_Level[column].size() - 1 >= row);

	int tileID{ m_Level[column][row] };
	int const nrOfTilesVertical{ 3 };
	int const nrOfTilesHorizontal{ 6 };

	dae::SourceRectangle result{};

	result.BaseWidth = nrOfTilesHorizontal * m_TileSide;
	result.BaseHeight = nrOfTilesVertical * m_TileSide;
	result.SrcWidth = m_TileSide;
	result.SrcHeight = m_TileSide;
	result.SrcPosY = static_cast<float>(tileID % nrOfTilesVertical) * result.SrcHeight;
	result.SrcPosX = static_cast<float>(tileID / nrOfTilesVertical) * result.SrcWidth;

	return result;
}

void dae::LevelComponent::CreateLevel()
{
	int const nrOfColumns{ 7 };
	int const nrOfRows{ 7 };

	for (int column = 0; column < nrOfColumns; column++)
	{
		std::vector<int> collumn;
		for (int row = 0; row < (nrOfRows - column); row++)
		{
			collumn.emplace_back(m_LevelData.m_TileSet * 3);
		}
		m_Level.emplace_back(collumn);
	}
}

void dae::LevelComponent::Render() const
{
	//auto& renderer = Renderer::GetInstance();
	auto textureCpnt = GetOwner()->GetComponent<dae::TextureComponent>();
	auto transformCpnt = GetOwner()->GetComponent<dae::TransformComponent>();

	if (textureCpnt == nullptr) return;
	if (transformCpnt == nullptr) return;

	float const tileSize{ m_TileSide * textureCpnt->GetRenderScale() };


	for (int column = 0; column < m_Level.size(); column++)
	{
		for (int row = 0; row < m_Level[column].size(); row++)
		{
			dae::SourceRectangle srcRect = GetSourceRect(column, row);
			textureCpnt->SetSourceRectangle(srcRect);
			transformCpnt->SetWorldPosition(m_BasePosition.x + (row * (tileSize * 0.5f)) - (column * (tileSize * 0.5f)), m_BasePosition.y + (row * (tileSize * 0.75f)) + (column * (tileSize * 0.75f)));
			textureCpnt->Render();
		}
	}
}

bool dae::LevelComponent::DoesTileExist(int column, int row)
{
	if (m_Level.size() <= column or column < 0) return false;

	if (m_Level[column].size() <= row or row < 0) return false;

	return true;
}

void dae::LevelComponent::Notify(const Event& event)
{
	switch (event.m_type)
	{
	case EventType::IsTileValid:
		{
			auto arguments{ event.GetArgumentsAsTuple<dae::TileCoordinates, dae::TileCoordinates>() };
			auto pos	{ std::get<0>(arguments) };
			auto direction{ std::get<1>(arguments) };

			std::tuple<bool, glm::vec3, dae::TileCoordinates> newEventArguments{ DoesTileExist(pos.m_Column + direction.m_Column, pos.m_Row + direction.m_Row), ConvertToWorld(direction.m_Column, direction.m_Row), direction };

			Event eventToNotify{ dae::EventType::ConfirmMovement, newEventArguments, -1 };

			dae::EventManager::GetInstance().PushEvent(eventToNotify);

			break;

		}
	case EventType::ToggleTile:
		{
			auto arguments{ event.GetArgumentsAsTuple<dae::TileCoordinates, dae::TileCoordinates>() };
			auto pos	{ std::get<0>(arguments) };
			auto direction{ std::get<1>(arguments) };
			ToggleTile(pos.m_Column, pos.m_Row);
			break;
			
		}
	case EventType::LoadNextLevel:
		LoadNewRound();
		break;
	}
}

glm::vec3 dae::LevelComponent::ConvertToWorld(int columnOffset, int rowOffset)
{
	glm::vec3 result{ static_cast<float>(columnOffset), static_cast<float>(rowOffset), 0 };
	result = glm::vec3(((result.x * 0.50f) - (result.y * 0.50f)), ((result.x * 0.75f) + (result.y * 0.75f)), 0);
	result *= m_TileSide * m_ZoomLevel;
	return result;
}

void dae::LevelComponent::ToggleTile(int column, int row)
{
	if (DoesTileExist(column, row))
	{
		int currentTileId{ m_Level[row][column] };

		if (m_LevelData.m_AllowTileDecrease)
		{
			int ZeroToggleID{ m_LevelData.m_TileSet * 3 };
			int tempTileID{ m_Level[row][column] - ZeroToggleID };

			tempTileID++;

			tempTileID = tempTileID % (m_LevelData.m_MaxToggles + 1);

			tempTileID += ZeroToggleID;

			m_Level[row][column] = tempTileID;
		}
		else if (currentTileId % 3 < m_LevelData.m_MaxToggles)
		{
			m_Level[row][column] += 1;
		}

		if (IsLevelFinished())
		{
			LoadNewRound();
		}
	}
}

void dae::LevelComponent::LoadNewRound()
{
	auto newRound{ LevelLoader::GetInstance().GetNextRound(m_LevelData.m_Level, m_LevelData.m_Round) };
	m_LevelData = newRound;

	for (int i = 0; i < m_Level.size(); i++)
	{
		m_Level[i].clear();
	}
	m_Level.clear();

	CreateLevel();
}

bool dae::LevelComponent::IsLevelFinished()
{
	int targetID{ (m_LevelData.m_TileSet * 3) + m_LevelData.m_MaxToggles };

	for (int  column = 0; column < m_Level.size(); column++)
	{
		for (int row = 0; row < m_Level[column].size(); row++)
		{
			if (m_Level[column][row] != targetID) return false;
		}
	}

	return true;
}
