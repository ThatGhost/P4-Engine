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
		byte* m_Argument = nullptr;

		virtual void OnDie(byte*) {}
		virtual void OnSalt(byte*) {}
		virtual void OnBurgderDone(byte*) {}

		virtual void Restart() {}
		virtual void GameOver() {}
		virtual void OnWin() {}
	};
}