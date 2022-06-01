#pragma once
#include <map>
#include <queue>
#include <functional>

namespace dae
{
	using byte = unsigned char;
	class EventManager
	{
	public:

		static void SendEvent(const std::string& EventName)
		{
			//std::cout << std::to_string((int)b) << std::endl;
			m_EventQue.push(EventName);
		}
		static void AddEvent(const std::string& EventName, std::function<void()> func)
		{
			if (!m_Events.contains(EventName))
			{
				m_Events.insert(std::pair(EventName, std::vector<std::function<void()>>{}));
				m_Events[EventName].push_back(func);
			}
			else
				m_Events[EventName].push_back(func);

		}
		static void AddEvent(const std::string& EventName, void(*func)())
		{
			if (!m_Events.contains(EventName))
				m_Events.insert(std::pair(EventName, std::vector<std::function<void()>>{}));
			else
				m_Events[EventName].push_back(func);

			m_Events[EventName].push_back(func);
		}
		static void ClearEvents()
		{
			m_Events.clear();
		}
		static void RemoveEvent(const std::string& EventName)
		{
			m_Events.erase(EventName);
		}
		static void ClearQueue()
		{
			while (!m_EventQue.empty())
			{
				std::string eventname = m_EventQue.front();
				if (m_Events.contains(eventname))
				{
					//std::cout << "event triggered: " << &m_Events[eventname][0] << '\n';
					for (size_t i{}; i < m_Events[eventname].size(); i++)
					{
						m_Events[eventname][i]();
					}
				}
				m_EventQue.pop();
			}
		}
	private:

		static std::queue<std::string> m_EventQue;
		static std::map<std::string, std::vector<std::function<void()>>> m_Events;
	};
}