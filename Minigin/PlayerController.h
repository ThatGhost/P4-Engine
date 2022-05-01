#pragma once
#include "Component.h"
namespace dae
{
	class PlayerController : public Component
	{
	public:
		PlayerController(GameObject*);
		virtual void Update(float) override;
		void Init(int player) { m_Player = player; }

	private:
		void Left(float);
		void Right(float);
		void Up(float);
		void Down(float);
		void OnCollisionPlatformL(float);
		void OnCollisionPlatformR(float);
		void OnCollisionStairsT(float);
		void OnCollisionStairsB(float);

		int m_Player{};
		float m_Argument{};
		glm::vec2 m_Movement{};
		bool m_OnStairsTop{};
		bool m_OnStairsBottom{};
		bool m_OnPlatformLeft{};
		bool m_OnPlatformRight{};

		const float m_Speed{100};
	};
}

