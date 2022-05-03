#include "MiniginPCH.h"
#include "SoundManager.h"
#include "dirent.h"

dae::SoundManager::~SoundManager()
{
	m_engine->drop();
}

void dae::SoundManager::Init()
{
	m_engine = irrklang::createIrrKlangDevice();
	if (!m_engine)
	{
		throw new std::runtime_error("Mixer did not initialize properly");
	}
}

void dae::SoundManager::PlaySound(const std::string& nameSound)
{
	std::string s{ "..\\Data\\Sound\\" + nameSound };
	m_engine->play2D(s.c_str());
}