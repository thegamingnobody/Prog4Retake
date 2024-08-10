#pragma once

#include <string>
#include "Singleton.h"
#include <vector>
#include "QbertStructs.h"
 
namespace dae
{
	class LevelLoader final : public Singleton<LevelLoader>
	{
	public:
		void ReadAndSaveLevel(std::string filePath);

		RoundData GetRound(int level, int round);
		RoundData GetNextRound(int level, int round);

	private:
		friend class Singleton<LevelLoader>;
		LevelLoader() = default;
		std::vector<std::vector<RoundData>> m_Levels;
	};

}
