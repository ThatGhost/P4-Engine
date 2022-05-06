#include "MiniginPCH.h"
#include "EventManager.h"

std::queue<std::pair<std::string, dae::byte*>> dae::EventManager::m_EventQue{};
std::map<std::string, std::vector<std::function<void(dae::byte*)>>> dae::EventManager::m_Events{};