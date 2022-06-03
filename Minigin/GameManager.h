#pragma once
#include "Component.h"
#include "SceneManager.h"

namespace dae
{
	using byte = unsigned char;
	class GameManager : public Component
	{
	public:
		GameManager(GameObject* owner) : Component(owner) {}
		virtual void AddScore(int) {}

	protected:
		virtual void OnDie() {}
		virtual void OnPepper() {}
		virtual void OnBurgderDone() {}

		virtual void Restart() {}
		virtual void GameOver() {}
		virtual void OnWin() {}
	};
}