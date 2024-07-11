#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();
	
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, float x, float y, float width, float height, const SourceRectangle& srcRect, float renderScale) const
{
	SDL_Rect dst{};
	SDL_Rect src{};

	float textureWidth{ width };
	float textureHeight{ height };
	if (textureWidth == 0 or textureHeight == 0)
	{
		textureWidth = srcRect.SrcWidth;
		textureHeight = srcRect.SrcHeight;
		//textureWidth = static_cast<float>(texture.GetSize().x);
		//textureHeight = static_cast<float>(texture.GetSize().y);
	}


	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(renderScale * textureWidth);
	dst.h = static_cast<int>(renderScale * textureHeight);
	
	src.x = static_cast<int>(srcRect.SrcPosX);
	src.y = static_cast<int>(srcRect.SrcPosY);
	src.w = static_cast<int>(srcRect.SrcWidth);
	src.h = static_cast<int>(srcRect.SrcHeight);

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
