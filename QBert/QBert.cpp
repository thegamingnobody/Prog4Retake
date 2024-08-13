#include <SDL.h>

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"

#include "CommandIncludes.h"
#include "ComponentIncludes.h"
#include "ManagerIncludes.h"

#include <Scene.h>


void load()
{
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::DAE_SDL_SoundSystem>());

	std::vector<std::string> m_SoundfilePaths{ "../Data/Sounds/QBert Jump.wav", "../Data/Sounds/Round Complete Tune.wav", "../Data/Sounds/Swearing.wav", "../Data/Sounds/Coily Egg Jump.wav" };

	for (int soundIndex = 0; soundIndex < static_cast<int>(m_SoundfilePaths.size()); soundIndex++)
	{
		dae::ServiceLocator::GetSoundSystem().AddSound(m_SoundfilePaths[soundIndex]);
	}
	
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& inputManager = dae::InputManager::GetInstance();
	auto& levelLoader = dae::LevelLoader::GetInstance();

#pragma region standard
	//auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto go = std::make_shared<dae::GameObject>("Prog4 assignment text");
	//go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	//go->AddComponent<dae::TransformComponent>(80.0f, 20.0f);
	//scene.Add(go);
#pragma endregion
	const float tileSize{ 32.0f };
	const float globalZoom{ 2.0f };
#pragma region player
	
		auto curseGo = std::make_shared<dae::GameObject>("PlayerCurse", 1);
	{
		dae::SourceRectangle sourceRect = dae::SourceRectangle(48.0f, 25.0f, 48.0f, 25.0f, 0.0f, 0.0f);

		auto& textureComponent = curseGo->AddComponent<dae::TextureComponent>("Curse.png", sourceRect, globalZoom * 0.50f);
		textureComponent.ToggleRender();
		auto textureDimentions = textureComponent.GetSize();
		curseGo->AddComponent<dae::TransformComponent>(	static_cast<float>(dae::Minigin::m_WindowWidth  * 0.50f) - (textureDimentions.x * 0.00f),
														static_cast<float>(dae::Minigin::m_WindowHeight * 0.25f) - (textureDimentions.y * 2.00f));
		curseGo->AddComponent<dae::CurseComponent>();
	}
	

	auto playerGo = std::make_shared<dae::GameObject>("Player", 1);
	{
		dae::SourceRectangle sourceRect = dae::SourceRectangle(64.0f, 16.0f, 16.0f, 16.0f, 48.0f, 0.0f);

		auto& textureComponent = playerGo->AddComponent<dae::TextureComponent>("Qbert1.png", sourceRect, globalZoom);
		auto textureDimentions = textureComponent.GetSize();

		playerGo->AddComponent<dae::TransformComponent>(	static_cast<float>(dae::Minigin::m_WindowWidth  * 0.50f) - (textureDimentions.x * 0.00f),
															static_cast<float>(dae::Minigin::m_WindowHeight * 0.25f) - (textureDimentions.y * 1.50f));
		
	}
	auto& playerComponent = playerGo->AddComponent<dae::QbertComponent>(curseGo.get());


#pragma endregion

#pragma region level

	{
		go = std::make_shared<dae::GameObject>("Level");

		levelLoader.ReadAndSaveLevel("../Data/Level1.txt");
		levelLoader.ReadAndSaveLevel("../Data/Level2.txt");
		levelLoader.ReadAndSaveLevel("../Data/Level3.txt");

		dae::RoundData currentLevel{ levelLoader.GetRound(1, 1) };

		if (currentLevel.m_IsValid)
		{
			dae::SourceRectangle sourceRect = dae::SourceRectangle(192.0f, 96.0f, tileSize, tileSize, tileSize * currentLevel.m_TileSet, 0.0f);

			auto& textureComponent = go->AddComponent<dae::TextureComponent>("Tiles.png", sourceRect, globalZoom);
			auto textureDimentions = textureComponent.GetSize();

			go->AddComponent<dae::TransformComponent>(	static_cast<float>(dae::Minigin::m_WindowWidth  * 0.50f) - (tileSize * 0.50f),
														static_cast<float>(dae::Minigin::m_WindowHeight * 0.25f) - (tileSize * 0.50f));
	
			/*auto& levelCpnt =*/ go->AddComponent<dae::LevelComponent>(dae::TileData(tileSize, globalZoom), currentLevel, playerComponent);
			
			//levelCpnt.LoadNewRound();

			scene.Add(go);
		}

	}
#pragma endregion
	
	scene.Add(curseGo);
	scene.Add(playerGo);

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


	inputManager.AddAction(dae::KeyboardKeys::F1, dae::InputType::PressedThisFrame, std::make_shared<dae::SkipLevelCommand>(), keyboardIndex);
	inputManager.AddAction(dae::KeyboardKeys::M, dae::InputType::PressedThisFrame, std::make_shared<dae::MuteCommand>(), keyboardIndex);

#pragma endregion

#pragma region Enemies
	
	//{
	//	go = std::make_shared<dae::GameObject>("Coily", 2);
	//	dae::SourceRectangle sourceRect = dae::SourceRectangle(160.0f, 32.0f, 16.0f, 32.0f, 16.0f, 0.0f);

	//	auto& textureComponent = go->AddComponent<dae::TextureComponent>("Coily.png", sourceRect, globalZoom);
	//	auto textureDimentions = textureComponent.GetSize();

	//	go->AddComponent<dae::TransformComponent>(	static_cast<float>(dae::Minigin::m_WindowWidth  * 0.50f) - (textureDimentions.x * 0.00f),
	//												static_cast<float>(dae::Minigin::m_WindowHeight * 0.25f) - (textureDimentions.y * 1.60f));
	//	
	//	go->AddComponent<dae::CoilyComponent>(playerComponent);

	//	scene.Add(go);
	//}

#pragma endregion


#pragma region HUD
	auto MCFont = dae::ResourceManager::GetInstance().LoadFont("Minecraft.ttf", 24);
	
	{
		go = std::make_shared<dae::GameObject>("HUD Health", -1);
		go->AddComponent<dae::TextureComponent>("Heart.png");
		go->AddComponent<dae::TransformComponent>(	static_cast<float>(dae::Minigin::m_WindowWidth  * 0.10f),
													static_cast<float>(dae::Minigin::m_WindowHeight * 0.20f));
		go->AddComponent<dae::HUDComponent>(dae::HUDType::Lives);
		scene.Add(go);
	}
	{
		go = std::make_shared<dae::GameObject>("HUD Game Over", -1);
		auto& textureComponent = go->AddComponent<dae::TextureComponent>("Game Over Title.png");
		textureComponent.ToggleRender();
		auto textureDimentions = textureComponent.GetSize();
		go->AddComponent<dae::TransformComponent>(	static_cast<float>(dae::Minigin::m_WindowWidth  * 0.50f) - (textureDimentions.x * 0.50f),
													static_cast<float>(dae::Minigin::m_WindowHeight * 0.50f) - (textureDimentions.y * 0.50f));
		go->AddComponent<dae::HUDComponent>(dae::HUDType::GameOver);
		scene.Add(go);
	}
	
	
	{
		go = std::make_shared<dae::GameObject>("HUD Level Count", -1);
		//go->AddComponent<dae::TextureComponent>("Heart.png");
		go->AddComponent<dae::TextComponent>("Level:", MCFont);
		go->AddComponent<dae::TransformComponent>(	static_cast<float>(dae::Minigin::m_WindowWidth  * 0.80f),
													static_cast<float>(dae::Minigin::m_WindowHeight * 0.15f));
		go->AddComponent<dae::HUDComponent>(dae::HUDType::LevelCount);
		scene.Add(go);
	}
	{
		go = std::make_shared<dae::GameObject>("HUD Round Count", -1);
		//go->AddComponent<dae::TextureComponent>("Heart.png");
		go->AddComponent<dae::TextComponent>("Round:", MCFont);
		go->AddComponent<dae::TransformComponent>(	static_cast<float>(dae::Minigin::m_WindowWidth  * 0.80f),
													static_cast<float>(dae::Minigin::m_WindowHeight * 0.20f));
		go->AddComponent<dae::HUDComponent>(dae::HUDType::RoundCount);
		scene.Add(go);
	}

#pragma endregion
}


int main(int, char* []) 
{
	srand(static_cast<unsigned int>(time(nullptr)));

	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}