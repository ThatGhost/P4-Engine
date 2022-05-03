#include "MiniginPCH.h"
#include "EventManager.h"

std::queue<std::pair<std::string, float>> dae::EventManager::m_EventQue{};
std::map<std::string,std::vector<std::function<void(float)>>> dae::EventManager::m_Events{};

void dae::EventManager::AddEvent(const std::string& EventName, std::function<void (float)> func)
{
	if(!m_Events.contains(EventName))
		m_Events.emplace(EventName, std::vector<std::function<void(float)>>{});

	m_Events[EventName].push_back(func);
}

void dae::EventManager::AddEvent(const std::string& EventName, void(*func)(float))
{
	if (!m_Events.contains(EventName))
		m_Events.emplace(EventName, std::vector<std::function<void(float)>>{});

	m_Events[EventName].push_back(func);
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
			for (size_t i{}; i < m_Events[e].size(); i++)
			{
				m_Events[e][i](m_EventQue.front().second);
			}
		}
		m_EventQue.pop();
	}
}
