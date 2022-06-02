#include "MiniginPCH.h"
#include "SoundManager.h"
#include "dirent.h"
#include "SDL_mixer.h"

dae::SoundManager::~SoundManager()
{
	Mix_CloseAudio();
	Mix_Quit();
}

void dae::SoundManager::Init()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		printf("SDL2_Mixer could not be initialised\n");
	}
}

void dae::SoundManager::PlaySound(const std::string& nameSound, bool loop, int channel)
{
	if (!m_sound.contains(nameSound))
	{
		Mix_Chunk* sample;
		sample = Mix_LoadWAV_RW(SDL_RWFromFile((m_BasePath + nameSound).c_str(), "rb"), 1);
		if (!sample) {
			printf("Mix_LoadWAV_RW: %s\n", Mix_GetError());
		}
		else
		{
			m_sound.insert(std::pair<std::string, Mix_Chunk*>(nameSound, sample));
		}
	}

	if (loop)
	{
		Mix_PlayChannel(channel,m_sound[nameSound],-1);
	}
	else
	{
		Mix_PlayChannel(channel, m_sound[nameSound], 0);
	}
}

void dae::SoundManager::PauseSound(bool pause, int channel)
{
	if (pause)
		Mix_Pause(channel);
	else
		Mix_Resume(channel);
	//m_sound[nameSound]->setIsPaused(pauze);
}

void dae::SoundManager::RemoveSound(const std::string& nameSound)
{
	if (m_sound.contains(nameSound))
	{
		m_sound.erase(nameSound);
	}
}
