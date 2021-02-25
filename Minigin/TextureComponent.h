#pragma once
#include "RenderComponent.h"
#include "Texture2D.h"
#include "TransformComponent.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)


namespace dae
{
	class TextureComponent : public RenderComponent
	{
	public:
		TextureComponent(const GameObject* pOwner, const std::string& fileName);
		TextureComponent(const GameObject* pOwner);

		void Render() const override;

		void SetTexture(const std::shared_ptr<Texture2D>& pTexture);
		void SetTexture(const std::string& fileName);

		void SetPosition(float x, float y);

		glm::vec2 GetTextureDimensions() const;
	private:
		std::shared_ptr<Texture2D> m_pTexture;
	protected:
		TransformComponent* m_pTransform;
	};

}
