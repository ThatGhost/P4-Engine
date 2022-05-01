#pragma once
#include "Component.h"

namespace dae
{
	class Texture2D;
	class RenderComponent : public Component
	{
	public:
		RenderComponent(GameObject* owner);
		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		void SetImage(const std::string& Image, bool isAnimated = false);
		void SetAnimationDetails(int frames, float frameTime, float Scale);

	private:
		bool m_IsAnimated = false;
		std::shared_ptr<Texture2D> m_Image;
		int m_Frame = 0;
		int m_MaxFrames = 1;
		float m_Scale = 1;
		float m_Timer = 0;
		float m_FrameTime = 0.5f;
	};
}

