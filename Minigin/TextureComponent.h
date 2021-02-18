#pragma once
#include "RenderComponent.h"
#include "Texture2D.h"
#include "TransformComponent.h"

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

	private:
		std::shared_ptr<Texture2D> m_pTexture;
		TransformComponent* m_pTransform;
	};

}
