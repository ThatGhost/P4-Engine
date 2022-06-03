//#pragma once
#include "Singleton.h"
#include <map>

struct Mix_Chunk;

namespace dae
{
	class SoundManager : public Singleton<SoundManager>
	{
	public:
		void Init();
		void PlaySound(const std::string& nameSound, bool loop = false, int = -1);
		void PauseSound(bool pause = true, int = -1);
		void RemoveSound(const std::string& nameSound);
	private:
		friend class Singleton<SoundManager>;
		~SoundManager();

		const std::string m_BasePath = "..\\Data\\Sound\\";
		std::map<std::string, Mix_Chunk*> m_Sounds{};
	};
}

