#pragma once
#include "Component.h"
namespace dae
{
	using byte = unsigned char;
	class PlayerController : public Component
	{
	public:
		PlayerController(GameObject*);
		virtual void Update(float) override;
		virtual void OnCollision(Collider*,Collider*) override;
		void Init(int player) { m_Player = player; }

	private:
		void Left(byte*);
		void Right(byte*);
		void Up(byte*);
		void Down(byte*);
		//void OnCollisionPlatformL(byte*);
		//void OnCollisionPlatformR(byte*);
		//void OnCollisionStairsT(byte*);
		//void OnCollisionStairsB(byte*);

		int m_Player{};
		byte* m_Argument;
		glm::vec2 m_Movement{};
		bool m_OnStairsTop{};
		bool m_OnStairsBottom{};
		bool m_OnPlatformLeft{};
		bool m_OnPlatformRight{};

		const float m_Speed{100};
	};
}

