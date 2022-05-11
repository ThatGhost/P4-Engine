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

		template<class T> static void SendEvent(const std::string& EventName, T args)
		{
			byte* b = ConvertToCharArray<T>(args);
			//std::cout << std::to_string((int)b) << std::endl;
			m_EventQue.push(std::make_pair(EventName, b));
		}
		static void AddEvent(const std::string& EventName, std::function<void(byte*)> func)
		{
			if (!m_Events.contains(EventName))
			{
				m_Events.insert(std::pair(EventName, std::vector<std::function<void(byte*)>>{}));
				m_Events[EventName].push_back(func);
			}
			else
				m_Events[EventName].push_back(func);

		}
		static void AddEvent(const std::string& EventName, void(*func)(byte*))
		{
			if (!m_Events.contains(EventName))
				m_Events.insert(std::pair(EventName, std::vector<std::function<void(byte*)>>{}));
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
				std::string eventname = m_EventQue.front().first;
				if (m_Events.contains(eventname))
				{
					//std::cout << "event triggered: " << &m_Events[eventname][0] << '\n';
					for (size_t i{}; i < m_Events[eventname].size(); i++)
					{
						byte* b = m_EventQue.front().second;
						m_Events[eventname][i](b);
					}
				}
				delete m_EventQue.front().second;
				m_EventQue.pop();
			}
		}
	private:
		template<class T> static byte* ConvertToCharArray(T obj)
		{
			int bytesize{ sizeof(T) };
			byte* c_array = new byte[8];
			memcpy(c_array, &obj, bytesize);
			return c_array;
		}

		static std::queue<std::pair<std::string, byte*>> m_EventQue;
		static std::map<std::string, std::vector<std::function<void(byte*)>>> m_Events;
	};
}