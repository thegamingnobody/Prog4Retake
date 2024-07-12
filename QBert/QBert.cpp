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

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& inputManager = dae::InputManager::GetInstance();

#pragma region standard
	//auto go = std::make_shared<dae::GameObject>("Background Image");
	//go->AddComponent<dae::TextureComponent>("background.tga");
	//scene.Add(go);

	//go = std::make_shared<dae::GameObject>("DAE Logo");
	//go->AddComponent<dae::TextureComponent>("logo.tga");
	//go->AddComponent<dae::TransformComponent>(216.0f, 180.0f);
	//scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto go = std::make_shared<dae::GameObject>("Prog4 assignment text");
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	go->AddComponent<dae::TransformComponent>(80.0f, 20.0f);
	scene.Add(go);
#pragma endregion

#pragma region level
	const float tileSize{ 32.0f };
	const float globalZoom{ 2.0f };

	go = std::make_shared<dae::GameObject>("Level");
	dae::SourceRectangle sourceRect = dae::SourceRectangle(192.0f, 96.0f, tileSize, tileSize, 0.0f, 0.0f);
	auto textureDimentions = go->AddComponent<dae::TextureComponent>("Tiles.png", sourceRect, globalZoom).GetSize();
	go->AddComponent<dae::TransformComponent>(static_cast<float>(dae::Minigin::m_WindowWidth * 0.50f) - (tileSize * 0.50f),
		static_cast<float>(dae::Minigin::m_WindowHeight * 0.25f) - (tileSize * 0.50f));
	go->AddComponent<dae::LevelComponent>();
	scene.Add(go);
#pragma endregion

	int controllerIndex{ inputManager.AddInputDevice(dae::Action::DeviceType::Controller) };
	int keyboardIndex{ inputManager.AddInputDevice(dae::Action::DeviceType::Keyboard) };

	inputManager.AddAction(dae::ControllerButtons::DpadUp, dae::InputType::PressedThisFrame, std::make_shared<dae::MoveCommand>(controllerIndex, glm::vec3(0, -1, 0), globalZoom), controllerIndex);
	inputManager.AddAction(dae::ControllerButtons::DpadDown, dae::InputType::PressedThisFrame, std::make_shared<dae::MoveCommand>(controllerIndex, glm::vec3(0, 1, 0), globalZoom), controllerIndex);
	inputManager.AddAction(dae::ControllerButtons::DpadLeft, dae::InputType::PressedThisFrame, std::make_shared<dae::MoveCommand>(controllerIndex, glm::vec3(-1, 0, 0), globalZoom), controllerIndex);
	inputManager.AddAction(dae::ControllerButtons::DpadRight, dae::InputType::PressedThisFrame, std::make_shared<dae::MoveCommand>(controllerIndex, glm::vec3(1, 0, 0), globalZoom), controllerIndex);

	inputManager.AddAction(dae::KeyboardKeys::ArrowUp, dae::InputType::PressedThisFrame, std::make_shared<dae::MoveCommand>(keyboardIndex, glm::vec3(0, -1, 0), globalZoom), keyboardIndex);
	inputManager.AddAction(dae::KeyboardKeys::ArrowDown, dae::InputType::PressedThisFrame, std::make_shared<dae::MoveCommand>(keyboardIndex, glm::vec3(0, 1, 0), globalZoom), keyboardIndex);
	inputManager.AddAction(dae::KeyboardKeys::ArrowLeft, dae::InputType::PressedThisFrame, std::make_shared<dae::MoveCommand>(keyboardIndex, glm::vec3(-1, 0, 0), globalZoom), keyboardIndex);
	inputManager.AddAction(dae::KeyboardKeys::ArrowRight, dae::InputType::PressedThisFrame, std::make_shared<dae::MoveCommand>(keyboardIndex, glm::vec3(1, 0, 0), globalZoom), keyboardIndex);

#pragma region player
	go = std::make_shared<dae::GameObject>("Player", -1);
	sourceRect = dae::SourceRectangle(68.0f, 16.0f, 17.0f, 16.0f, 51.0f, 0.0f);
	textureDimentions = go->AddComponent<dae::TextureComponent>("Qbert1.png", sourceRect, globalZoom).GetSize();
	go->AddComponent<dae::TransformComponent>(static_cast<float>(dae::Minigin::m_WindowWidth * 0.50f) - (textureDimentions.x * 0.00f),
		static_cast<float>(dae::Minigin::m_WindowHeight * 0.25f) - (textureDimentions.y * 1.50f));
	scene.Add(go);
#pragma endregion

}


int main(int, char* []) 
{
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}