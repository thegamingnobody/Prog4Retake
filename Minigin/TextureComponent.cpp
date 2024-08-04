#include "TextureComponent.h"
#include "ResourceManager.h"
#include <iostream>
#include "Renderer.h"
#include "Transform.h"
#include "GameObject.h"
#include "TransformComponent.h"


void dae::TextureComponent::Render() const
{
	if (m_Texture != nullptr)
	{
		if (m_Render == true)
		{
			auto transformComp{ GetOwner()->GetComponent<dae::TransformComponent>() };
			if (transformComp)
			{
				auto& pos{ transformComp->GetPosition().GetPosition() };
				Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, 0, 0, m_SrcRect, m_RenderScale);
			}
			else
			{
				Renderer::GetInstance().RenderTexture(*m_Texture, 0.0f, 0.0f);
			}
		}
	}
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	try
	{
		auto isValidTexture{ ResourceManager::GetInstance().LoadTexture(filename) };
		m_Texture.reset();
		m_Texture = isValidTexture;
		isValidTexture.reset();
	}
	catch (...)
	{
		std::cout << "Invalid Texture: \"" << filename << "\"\n";
	}
}

void dae::TextureComponent::SetTexture(const std::shared_ptr<Texture2D>& texture)
{
	if (texture)
	{
		m_Texture = texture;
	}
}

void dae::TextureComponent::SetSourceRectangle(float srcPosX, float srcPosY, float srcWidth, float srcHeight)
{
	m_SrcRect.SrcPosX = srcPosX;
	m_SrcRect.SrcPosY = srcPosY;
	m_SrcRect.SrcWidth = srcWidth;
	m_SrcRect.SrcHeight = srcHeight;
}

void dae::TextureComponent::SetSourceRectangle(const SourceRectangle& newSourceRect)
{
	SetSourceRectangle(newSourceRect.SrcPosX, newSourceRect.SrcPosY, newSourceRect.SrcWidth, newSourceRect.SrcHeight);
}

void dae::TextureComponent::SetRenderScale(float newScale)
{
	if (newScale != 0)
	{
		m_RenderScale = newScale;
	}
}

dae::TextureComponent::TextureComponent(GameObject* object, const std::string& filename)
	: Component(object)
	, m_Texture()
	, m_SrcRect()
	, m_RenderScale(1)
	, m_Render(true)
{
	SetTexture(filename);

	float baseWidth		{ static_cast<float>(m_Texture->GetSize().x) };
	float baseHeight	{ static_cast<float>(m_Texture->GetSize().y) };
	m_SrcRect = SourceRectangle(baseWidth, baseHeight, baseWidth, baseHeight, 0.0f, 0.0f);
}

dae::TextureComponent::TextureComponent(dae::GameObject* object, const std::string& filename, const SourceRectangle& sourceRect, float renderScale)
	: Component(object)
	, m_Texture()
	, m_SrcRect()
	, m_RenderScale(renderScale)
	, m_Render(true)
{
	SetTexture(filename);

	float baseWidth		{ static_cast<float>(m_Texture->GetSize().x) };
	float baseHeight	{ static_cast<float>(m_Texture->GetSize().y) };
	float tempSrcWidth	{ sourceRect.SrcWidth };
	float tempSrcHeight	{ sourceRect.SrcHeight };
	if (sourceRect.SrcWidth == 0 or sourceRect.SrcHeight == 0)
	{
		tempSrcWidth = baseWidth;
		tempSrcHeight = baseHeight;
	}

	m_SrcRect = SourceRectangle(baseWidth, baseHeight , tempSrcWidth, tempSrcHeight, sourceRect.SrcPosX, sourceRect.SrcPosY);
}
