#pragma once
#include "Component.h"
namespace dae
{
	using byte = unsigned char;
	class RenderComponent;

	class PlayerController : public Component
	{
	public:
		PlayerController(GameObject*);
		~PlayerController() override;

		virtual void Update(float) override;
		virtual void OnCollision(Collider*,Collider*) override;

		void Init(int player);

	private:
		void Left();
		void Right();
		void Up();
		void Down();
		void Pepper();

		int m_Player{};
		RenderComponent* m_renderer{nullptr};

		glm::vec2 m_Movement{};

		bool m_OnStairsTop{};
		bool m_OnStairsBottom{};
		bool m_OnPlatformLeft{};
		bool m_OnPlatformRight{};

		float m_platformHeight{};
		float m_stairOffset{};

		const float m_Speed{50}; //pixels per second
		const std::string m_WalkingSound{"Footstep.wav"};
	};
}

