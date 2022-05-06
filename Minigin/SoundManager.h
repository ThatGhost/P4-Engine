#pragma once
#include "Singleton.h"
#include "irrKlang.h"

namespace dae
{
	class SoundManager : public Singleton<SoundManager>
	{
	public:
		void Init();
		void PlaySound(const std::string& nameSound);

	private:
		friend class Singleton<SoundManager>;
		SoundManager() = default;
		~SoundManager();

		const int m_Channels = 2;
		irrklang::ISoundEngine* m_engine = nullptr;
	};
}

