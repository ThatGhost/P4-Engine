#pragma once
#include "Component.h"

namespace dae {
	class Font;
	class Texture2D;

	class FpsComponent final: public Component
	{
	public:
		FpsComponent(GameObject* Owner);
		virtual ~FpsComponent() override;

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		//void PressedX(int);

	private:
		std::shared_ptr<Font> m_Font;
		Texture2D* m_TextTexture;
	};
}

