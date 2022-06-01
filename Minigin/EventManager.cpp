#include "MiniginPCH.h"
#include "EventManager.h"

std::queue<std::string> dae::EventManager::m_EventQue{};
std::map<std::string, std::vector<std::function<void()>>> dae::EventManager::m_Events{};