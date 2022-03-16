#pragma once
#include "Component.h"

namespace dae {
	class Font;
	class Texture2D;

	class FpsComponent : public Component
	{
	public:
		FpsComponent(GameObject* Owner);
		~FpsComponent();

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

	private:
		std::shared_ptr<Font> m_Font;
		Texture2D* m_TextTexture;
	};
}

