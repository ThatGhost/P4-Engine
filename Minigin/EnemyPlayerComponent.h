#pragma once
#include "Component.h"

namespace dae
{
	using byte = unsigned char;
	class RenderComponent;

	class EnemyPlayerComponent : public Component
	{
	public:
		EnemyPlayerComponent(GameObject*);
		~EnemyPlayerComponent() override;
		virtual void Update(float) override;
		virtual void Start() override;
		virtual void OnCollision(Collider*, Collider*) override;
		virtual void OnCollisionEnter(Collider*, Collider*) override;

	private:
		void Left();
		void Right();
		void Up();
		void Down();
		void Restart();

		int m_Player{1};
		RenderComponent* m_renderer{ nullptr };

		glm::vec2 m_Movement{};
		glm::vec3 m_direction{};

		bool m_OnStairsTop{};
		bool m_OnStairsBottom{};
		bool m_OnPlatformLeft{};
		bool m_OnPlatformRight{};

		float m_platformHeight{};
		float m_stairOffset{};
		glm::vec3 m_startpos{};

		bool m_peppered{false};
		float m_timer{};
		const float m_PepperTime{3};

		const float m_Speed{ 60 }; //pixels per second
		const float m_Friction{ 20 }; 
		const float m_acceleration{ 40 }; 
	};
}

