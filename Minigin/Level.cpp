#include "Level.h"
#include "Renderer.h"
#include "GameObject.h"
#include "TransformComponent.h"

dae::LevelComponent::LevelComponent(GameObject* ownerObject)
	: Component(ownerObject)
	, m_Level()
	, m_BasePosition()
{
	CreateLevel();
	auto transformCpnt = GetOwner()->GetComponent<dae::TransformComponent>();
	if (transformCpnt != nullptr)
	{
		m_BasePosition = transformCpnt->GetPosition().GetPosition();
	}
}

dae::SourceRectangle dae::LevelComponent::GetSourceRect(int column, int row) const
{
	assert(m_Level.size() - 1 >= column);
	assert(m_Level[column].size() - 1 >= row);

	int tileID{ m_Level[column][row] };
	float const tileSize{ 32.0f };
	int const nrOfTilesVertical{ 3 };
	int const nrOfTilesHorizontal{ 6 };

	dae::SourceRectangle result{};

	result.BaseWidth = nrOfTilesHorizontal * tileSize;
	result.BaseHeight = nrOfTilesVertical * tileSize;
	result.SrcWidth = tileSize;
	result.SrcHeight = tileSize;
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
			collumn.emplace_back(0);
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

	float const tileSize{ 32.0f * textureCpnt->GetRenderScale() };


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
	if (m_Level.size() < column or column < 0) return false;

	if (m_Level[column].size() < row or row < 0) return false;

	return true;
}
