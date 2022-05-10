#pragma once
#include "Component.h"
namespace dae
{
	using byte = unsigned char;
	class BurgerComponent : public Component
	{
	public:
		BurgerComponent(GameObject* Owner);
		virtual ~BurgerComponent() override;

		virtual void Update(float deltaTime) override;
		virtual void OnCollisionEnter(Collider*, Collider*) override;
		virtual void OnCollision(Collider*, Collider*) override;

		void SetIsFalling(bool isFalling) { m_isFalling = isFalling; }
		bool IsDone() { return m_done; }
		bool IsFalling() { return m_isFalling; }
	private:
		void AddScore(int);

		const float m_FallSpeed{8};
		const float m_MaxSpeed{40};

		bool m_walkSpots[5]{false};

		float m_fallVelocity{};
		bool m_isFalling{true};
		bool m_done{ false };
		bool m_ini{true};
		int m_ScoreToAdd{};
	};
}

