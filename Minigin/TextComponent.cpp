#include <stdexcept>
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "TransformComponent.h"
#include "GameObject.h"

dae::TextComponent::TextComponent(GameObject* ownerObject, const std::string& text, std::shared_ptr<Font> font)
	: Component(ownerObject)
	, m_needsUpdate(true)
	, m_text(text)
	, m_font(std::move(font))
{

}

void dae::TextComponent::Update(float const)
{
	if (m_needsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_textTexture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	if (m_textTexture != nullptr)
	{
		auto owner{ GetOwner() };
		if (owner == nullptr)
			return;

		auto transformCpntOwner{ owner->GetComponent<TransformComponent>() };
		if (transformCpntOwner == nullptr)
			return;

		const auto& pos = transformCpntOwner->GetPosition().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y, static_cast<float>(m_textTexture->GetSize().x), static_cast<float>(m_textTexture->GetSize().y), dae::SourceRectangle(static_cast<float>(m_textTexture->GetSize().x), static_cast<float>(m_textTexture->GetSize().y), static_cast<float>(m_textTexture->GetSize().x), static_cast<float>(m_textTexture->GetSize().y), 0.0f, 0.0f));
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

