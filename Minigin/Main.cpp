#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
//#include <vld.h>
#endif
#endif

#include "GameObject.h"
#include "Minigin.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
#include "Action.h"
#include "Level.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

#pragma region standard
	auto go = std::make_shared<dae::GameObject>("Background Image");
	go->AddComponent<dae::TextureComponent>("background.tga");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("DAE Logo");
	go->AddComponent<dae::TextureComponent>("logo.tga");
	go->AddComponent<dae::TransformComponent>(216.0f, 180.0f);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_shared<dae::GameObject>("Prog4 assignment text");
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	go->AddComponent<dae::TransformComponent>(80.0f, 20.0f);
	scene.Add(go);
#pragma endregion

#pragma region level
	const float tileSize{ 32.0f };

	go = std::make_shared<dae::GameObject>("Level");
	dae::SourceRectangle sourceRect = dae::SourceRectangle(192.0f, 96.0f, tileSize, tileSize, 0.0f, 0.0f);
	auto textureDimentions = go->AddComponent<dae::TextureComponent>("Tiles.png", sourceRect, 1.5f).GetSize();
	go->AddComponent<dae::TransformComponent>(	static_cast<float>(dae::Minigin::m_WindowWidth / 2) - (tileSize / 2),
												static_cast<float>(dae::Minigin::m_WindowHeight / 2) - (tileSize / 2));
	go->AddComponent<dae::LevelComponent>();
	scene.Add(go);
#pragma endregion

#pragma region player
	go = std::make_shared<dae::GameObject>("Player");
	sourceRect = dae::SourceRectangle(68.0f, 16.0f, 17.0f, 16.0f, 51.0f, 0.0f);
	textureDimentions = go->AddComponent<dae::TextureComponent>("Qbert1.png", sourceRect, 1.5f).GetSize();
	go->AddComponent<dae::TransformComponent>(	static_cast<float>(dae::Minigin::m_WindowWidth / 2) - (textureDimentions.x * 0.25f * 0.5f),
												static_cast<float>(dae::Minigin::m_WindowHeight / 2) - (textureDimentions.y * 1.5f));
	scene.Add(go);
#pragma endregion


	//dae::Action moveAction{ dae::ControllerButtons::DpadRight, dae::InputType::PressedThisFrame,  };
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}