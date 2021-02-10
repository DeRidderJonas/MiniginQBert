#pragma once
#include "RenderComponent.h"
#include "Texture2D.h"

namespace dae
{
	class TextureComponent : public RenderComponent
	{
	public:
		TextureComponent(const std::shared_ptr<GameObject>& pOwner, const std::string& fileName);
		virtual ~TextureComponent() = default;
		virtual void Render() const override;
		void SetTexture(const std::string& fileName);
	private:
		std::shared_ptr<Texture2D> m_pTexture;
	};

}
