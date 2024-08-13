#include "Scene.h"
#include "GameObject.h"

#include <algorithm>
#include <numeric>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update(float const deltaTime)
{
	bool shouldRemove{ false };

	for(auto& object : m_objects)
	{
		object->Update(deltaTime);
		if (object->ShouldRemove())
		{
			shouldRemove = true;
		}
	}

	if (shouldRemove)
	{
		std::erase_if(m_objects, [](std::shared_ptr<GameObject> object) { return object->ShouldRemove(); });
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

