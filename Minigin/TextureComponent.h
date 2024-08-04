#ifndef TEXTURECOMPONENT
#define TEXTURECOMPONENT

#include "Component.h"
#include <string>
#include "Texture2D.h"
#include <memory>

namespace dae
{
	struct SourceRectangle
	{
		float BaseWidth;
		float BaseHeight;
		float SrcWidth;
		float SrcHeight;
		float SrcPosX;
		float SrcPosY;

		SourceRectangle(float baseWidth, float baseHeight, float srcWidth, float srcHeight, float srcPosX, float srcPosY)
		{
			BaseWidth = baseWidth;
			BaseHeight = baseHeight;
			SrcWidth = srcWidth;
			SrcHeight = srcHeight;
			SrcPosX = srcPosX;
			SrcPosY = srcPosY;
		}
		SourceRectangle()
		{
			BaseWidth	= 1;
			BaseHeight	= 1;
			SrcWidth	= 1;
			SrcHeight	= 1;
			SrcPosX		= 0;
			SrcPosY		= 0;
		}

	};
	class TextureComponent : public Component
	{
	public:

		void Render() const;

		void SetTexture(const std::string& filename);
		void SetTexture(const std::shared_ptr<Texture2D>& texture);

		void SetSourceRectangle(float srcPosX, float srcPosY, float srcWidth, float srcHeight);
		void SetSourceRectangle(const SourceRectangle& newSourceRect);

		void SetRenderScale(float newScale);
		float GetRenderScale() const { return m_RenderScale; }

		glm::ivec2 GetSize() const { return m_Texture->GetSize(); }

		void ToggleRender() { m_Render = not(m_Render); }
		void ToggleRender(bool render) { m_Render = render; }

		TextureComponent(GameObject* object, const std::string& filename);
		TextureComponent(GameObject* object, const std::string& filename, const SourceRectangle& sourceRect, float renderScale = 1);
		virtual ~TextureComponent() override = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator= (const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

	private:
		std::shared_ptr<Texture2D> m_Texture;
		SourceRectangle m_SrcRect;
		float m_RenderScale;

		bool m_Render;
	};
}

#endif