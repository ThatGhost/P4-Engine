#include "MiniginPCH.h"
#include "SoundManager.h"
#include "dirent.h"
#pragma comment(lib, "irrKlang.lib")

using namespace irrklang;

dae::SoundManager::~SoundManager()
{
	m_engine->drop();
	for (auto& e : m_sound)
	{
		e.second->drop();
	}
}

void dae::SoundManager::Init()
{
	m_engine = createIrrKlangDevice();
	if (!m_engine)
	{
		throw new std::runtime_error("Mixer did not initialize properly");
	}
	m_engine->setSoundVolume(0.1f);
}

void dae::SoundManager::PlaySound(const std::string& nameSound, bool loop)
{
	if (!m_sources.contains(nameSound))
	{
		std::string s{ "..\\Data\\Sound\\" + nameSound };
		ISoundSource* sound = m_engine->addSoundSourceFromFile(s.c_str());
		m_sources.insert(std::pair(nameSound,sound));
	}
	if (loop)
	{
		irrklang::ISound* sound = m_engine->play2D(m_sources[nameSound], loop, false, true);
		m_sound.insert(std::pair(nameSound, sound));
	}
	else
		m_engine->play2D(m_sources[nameSound], loop);
}

void dae::SoundManager::PauseSound(const std::string& nameSound, bool pauze)
{
	m_sound[nameSound]->setIsPaused(pauze);
}

void dae::SoundManager::RemoveSound(const std::string& nameSound)
{
	if (m_sound.contains(nameSound))
	{
		m_sound[nameSound]->setIsPaused();
		m_sound[nameSound]->setIsLooped(false);
		m_sound[nameSound]->drop();
		m_sound.erase(nameSound);
	}
}
