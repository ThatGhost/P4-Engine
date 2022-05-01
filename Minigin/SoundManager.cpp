#include "MiniginPCH.h"
#include "SoundManager.h"
#include "dirent.h"
#include "SDL_mixer.h"

dae::SoundManager::~SoundManager()
{
	Mix_CloseAudio();
}

void dae::SoundManager::Init()
{
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, m_Channels, 4096) == -1)
	{
		throw new std::runtime_error("Mixer did not initialize properly");
	}
	LoadFiles();
}

void dae::SoundManager::PlaySound()
{

}

void dae::SoundManager::LoadFiles()
{
	struct dirent* entry;
	std::string ObjectAssetPath{ "..\\Data\\Sound\\" };

	DIR* dir = opendir(ObjectAssetPath.c_str());

	if (dir == NULL) {
		return;
	}

	while ((entry = readdir(dir)) != NULL)
	{
		std::cout << entry->d_name << '\n';
	}
	closedir(dir);

	m_TestSound = Mix_LoadWAV("..\\Data\\Sounds\\Levelup.wav");
}