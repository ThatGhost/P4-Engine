#pragma once
#include "Singleton.h"

class Mix_Chunk;
namespace dae
{
	class SoundManager : public Singleton<SoundManager>
	{
	public:
		void Init();
		void PlaySound();

	private:
		friend class Singleton<SoundManager>;
		SoundManager() = default;
		~SoundManager();

		const int m_Channels = 2;
		Mix_Chunk* m_TestSound;

		void LoadFiles();
	};
}

