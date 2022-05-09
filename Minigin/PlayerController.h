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
		RenderComponent* m_renderer{nullptr};

		glm::vec2 m_Movement{};
		bool m_OnStairsTop{};
		bool m_OnStairsBottom{};
		bool m_OnPlatformLeft{};
		bool m_OnPlatformRight{};
		float m_platformHeight{};
		float m_stairOffset{};

		const float m_Speed{50}; //pixels per second
	};
}

