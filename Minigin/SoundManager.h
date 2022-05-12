//#pragma once
#include "Singleton.h"
//#include <irrKlang.h>
//#include <map>
//
namespace dae
{
	class SoundManager : public Singleton<SoundManager>
	{
	public:
		void Init() {};
		void PlaySound(const std::string&, bool = false) {};
		void PauseSound(const std::string&, bool = false){};
		void RemoveSound(const std::string&) {};
	private:
		friend class Singleton<SoundManager>;
		SoundManager() = default;
		~SoundManager() {};

		//const int m_Channels = 2;
		//irrklang::ISoundEngine* m_engine = nullptr;
		//std::map<std::string, irrklang::ISoundSource*> m_sources{};
		//std::map<std::string, irrklang::ISound*> m_sound{};
	};
}
//
