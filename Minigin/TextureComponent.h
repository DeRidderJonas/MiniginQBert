#pragma once
#include "RenderComponent.h"
#include "Texture2D.h"
#include "TransformComponent.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)
#include <string>

namespace dae
{
	class TextureComponent : public RenderComponent
	{
	public:
		TextureComponent(GameObject* pOwner, const std::string& fileName);
		TextureComponent(GameObject* pOwner);

		void Render() const override;
		void Initialize() override;

		void SetTexture(const std::shared_ptr<Texture2D>& pTexture);
		void SetTexture(const std::string& fileName);

		void SetPosition(float x, float y);

		glm::vec2 GetTextureDimensions() const;

		void SetOverlayColor(const glm::vec3& color);
	private:
		std::shared_ptr<Texture2D> m_pTexture;
	protected:
		TransformComponent* m_pTransform;
	};

}
