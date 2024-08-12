#include "LevelLoader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

void dae::LevelLoader::ReadAndSaveLevel(std::string filePath)
{

	std::ifstream levelFile{ filePath };
	if (!levelFile)
	{
		std::cout << "cannot open or find file\n";
		return;
	}
	
	std::vector<RoundData> currentLevel;
	std::string line;
	dae::RoundData result{};
	while (std::getline(levelFile, line))
	{
		std::istringstream input{ line };
		std::string tag;
		while (input >> tag)
		{
			if (tag == "Round")
			{
				input >> result.m_Round;
			}
			else if (tag == "Level")
			{
				input >> result.m_Level;
			}
			else if (tag == "Tileset")
			{
				input >> result.m_TileSet;
			}
			else if (tag == "MaxToggles")
			{
				input >> result.m_MaxToggles;
			}
			else if (tag == "AllowDecreaseTile")
			{
				input >> result.m_AllowTileDecrease;
			}
			else if (tag == "End")
			{
				currentLevel.emplace_back(result);
			}
		}
	}
	m_Levels.emplace_back(currentLevel);

	levelFile.close();
}

dae::RoundData dae::LevelLoader::GetRound(int level, int round)
{
	assert(level > 0 and round > 0);
	return m_Levels[level-1][round-1];
}

dae::RoundData dae::LevelLoader::GetNextRound(int level, int round)
{
	assert(level > 0 and round > 0);
	assert(level <= m_Levels.size());

	if (round == m_Levels[level-1].size())
	{
		if (level == m_Levels.size())
		{
			return GetRound(level, round);
		}
		else
		{
			return GetRound(level + 1, 1);
		}
	}
	else
	{
		return GetRound(level, round + 1);
	}

}
