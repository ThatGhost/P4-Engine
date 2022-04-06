#pragma once
#include <map>
#include <queue>
#include <functional>

namespace dae
{
	class EventManager
	{
	public:

		static void AddEvent(const std::string& EventName, std::function<void (float)> func);
		static void SendEvent(const std::string& EventName, float args);
		static void ClearQueue();

	private:
		static std::queue<std::pair<std::string, float>> m_EventQue;
		static std::map<std::string, std::function<void(float)>> m_Events;
	};
}