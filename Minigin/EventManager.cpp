#include "MiniginPCH.h"
#include "EventManager.h"

std::queue<std::pair<std::string, float>> dae::EventManager::m_EventQue{};
std::map<std::string, std::function<void(float)>> dae::EventManager::m_Events{};

void dae::EventManager::AddEvent(const std::string& EventName, std::function<void (float)> func)
{
	if (!m_Events.contains(EventName))
		m_Events.emplace(EventName, func);
}

void dae::EventManager::SendEvent(const std::string& EventName, float args)
{
	m_EventQue.push(std::make_pair(EventName, args));
}

void dae::EventManager::ClearQueue()
{
	while (!m_EventQue.empty())
	{
		std::string e = m_EventQue.front().first;
		if (m_Events.contains(e))
		{
			m_Events[e](m_EventQue.front().second);
		}
		m_EventQue.pop();
	}
}
