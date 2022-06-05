#pragma once
#include "Component.h"


namespace dae
{
	class MatchComponent :	public Component
	{
	public:
		MatchComponent(GameObject* owner) : Component(owner)
		{
		
		}

		~MatchComponent() override {};

		void Init(const std::string& ip, uint16_t port) { m_Ipv4 = ip; m_Port = port; }
		std::string getIp() const { return m_Ipv4; }
		uint16_t GetPort() const { return m_Port; }
	private:
		std::string m_Ipv4;
		uint16_t m_Port;
	};
}

