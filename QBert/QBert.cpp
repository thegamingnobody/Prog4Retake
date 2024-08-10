#include <SDL.h>

#if _DEBUG
#if __has_include(<vld.h>)
//#include <vld.h>
#endif
#endif

#include "Minigin.h"

#include "CommandIncludes.h"
#include "ComponentIncludes.h"
#include "ManagerIncludes.h"

#include <Scene.h>
#include "QbertComponent.h"
#include "CurseComponent.h"

#include <ServiceLocator.h>
#include <soundSystem.h>
#include <DAE_SDL_SoundSystem.h>

#include <fstream>
#include <sstream>

dae::LevelData LoadLevel(std::string filePath);

void load()
{
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::DAE_SDL_SoundSystem>());

	std::vector<std::string> m_SoundfilePaths{ "../Data/Sounds/QBert Jump.wav" };

	for (int soundIndex = 0; soundIndex < m_SoundfilePaths.size(); soundIndex++)
	{
		dae::ServiceLocator::GetSoundSystem().AddSound(m_SoundfilePaths[soundIndex]);
	}
	
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& inputManager = dae::InputManager::GetInstance();

#pragma region standard
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto go = std::make_shared<dae::GameObject>("Prog4 assignment text");
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	go->AddComponent<dae::TransformComponent>(80.0f, 20.0f);
	scene.Add(go);
#pragma endregion

#pragma region level
	const float tileSize{ 32.0f };
	const float globalZoom{ 2.0f };
	{
		go = std::make_shared<dae::GameObject>("Level");

		dae::LevelData currentLevel{ LoadLevel("../Data/Level1.txt") };

		if (currentLevel.m_IsValid)
		{
			dae::SourceRectangle sourceRect = dae::SourceRectangle(192.0f, 96.0f, tileSize, tileSize, tileSize * currentLevel.m_Tileset, 0.0f);

			auto& textureComponent = go->AddComponent<dae::TextureComponent>("Tiles.png", sourceRect, globalZoom);
			auto textureDimentions = textureComponent.GetSize();

			go->AddComponent<dae::TransformComponent>(	static_cast<float>(dae::Minigin::m_WindowWidth  * 0.50f) - (tileSize * 0.50f),
														static_cast<float>(dae::Minigin::m_WindowHeight * 0.25f) - (tileSize * 0.50f));
	
			go->AddComponent<dae::LevelComponent>(dae::TileData(tileSize, globalZoom), currentLevel);

			scene.Add(go);
		}

	}

#pragma endregion

#pragma region Input

	int controllerIndex{ inputManager.AddInputDevice(dae::Action::DeviceType::Controller) };
	int keyboardIndex{ inputManager.AddInputDevice(dae::Action::DeviceType::Keyboard) };

	inputManager.AddAction(dae::ControllerButtons::DpadUp,		dae::InputType::PressedThisFrame, std::make_shared<dae::MoveCommand>(controllerIndex,	glm::vec3( 0, -1,  0)), controllerIndex);
	inputManager.AddAction(dae::ControllerButtons::DpadDown,	dae::InputType::PressedThisFrame, std::make_shared<dae::MoveCommand>(controllerIndex,	glm::vec3( 0,  1,  0)), controllerIndex);
	inputManager.AddAction(dae::ControllerButtons::DpadLeft,	dae::InputType::PressedThisFrame, std::make_shared<dae::MoveCommand>(controllerIndex,	glm::vec3(-1,  0,  0)), controllerIndex);
	inputManager.AddAction(dae::ControllerButtons::DpadRight,	dae::InputType::PressedThisFrame, std::make_shared<dae::MoveCommand>(controllerIndex,	glm::vec3( 1,  0,  0)), controllerIndex);

	inputManager.AddAction(dae::KeyboardKeys::ArrowUp,			dae::InputType::PressedThisFrame, std::make_shared<dae::MoveCommand>(keyboardIndex,		glm::vec3( 0, -1,  0)), keyboardIndex);
	inputManager.AddAction(dae::KeyboardKeys::ArrowDown,		dae::InputType::PressedThisFrame, std::make_shared<dae::MoveCommand>(keyboardIndex,		glm::vec3( 0,  1,  0)), keyboardIndex);
	inputManager.AddAction(dae::KeyboardKeys::ArrowLeft,		dae::InputType::PressedThisFrame, std::make_shared<dae::MoveCommand>(keyboardIndex,		glm::vec3(-1,  0,  0)), keyboardIndex);
	inputManager.AddAction(dae::KeyboardKeys::ArrowRight,		dae::InputType::PressedThisFrame, std::make_shared<dae::MoveCommand>(keyboardIndex,		glm::vec3( 1,  0,  0)), keyboardIndex);

#pragma endregion

#pragma region player
	
		auto curseGo = std::make_shared<dae::GameObject>("PlayerCurse", 5);
	{
		dae::SourceRectangle sourceRect = dae::SourceRectangle(48.0f, 25.0f, 48.0f, 25.0f, 0.0f, 0.0f);

		auto& textureComponent = curseGo->AddComponent<dae::TextureComponent>("Curse.png", sourceRect, globalZoom * 0.50f);
		textureComponent.ToggleRender();
		auto textureDimentions = textureComponent.GetSize();
		curseGo->AddComponent<dae::TransformComponent>(	static_cast<float>(dae::Minigin::m_WindowWidth  * 0.50f) - (textureDimentions.x * 0.00f),
														static_cast<float>(dae::Minigin::m_WindowHeight * 0.25f) - (textureDimentions.y * 2.00f));
		curseGo->AddComponent<dae::CurseComponent>();
		scene.Add(curseGo);
	}
	

	{
		go = std::make_shared<dae::GameObject>("Player", -1);
		dae::SourceRectangle sourceRect = dae::SourceRectangle(68.0f, 16.0f, 17.0f, 16.0f, 51.0f, 0.0f);

		auto& textureComponent = go->AddComponent<dae::TextureComponent>("Qbert1.png", sourceRect, globalZoom);
		auto textureDimentions = textureComponent.GetSize();

		go->AddComponent<dae::TransformComponent>(	static_cast<float>(dae::Minigin::m_WindowWidth  * 0.50f) - (textureDimentions.x * 0.00f),
													static_cast<float>(dae::Minigin::m_WindowHeight * 0.25f) - (textureDimentions.y * 1.50f));
		
		go->AddComponent<dae::QbertComponent>(curseGo.get());

		scene.Add(go);
	}

#pragma endregion

}


int main(int, char* []) 
{
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}

dae::LevelData LoadLevel(std::string filePath)
{
	dae::LevelData result{};

	std::ifstream levelFile{ filePath };
	if (!levelFile)
	{
		std::cout << "cannot open or find file\n";
		result.m_IsValid = false;
		return result;
	}


	std::string line;
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
			else if (tag == "Tileset")
			{
				input >> result.m_Tileset;
			}
			else if (tag == "MaxToggles")
			{
				input >> result.m_MaxToggles;
			}

		}
	}
	levelFile.close();

	return result;
}
