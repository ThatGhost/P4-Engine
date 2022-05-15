#pragma once
#include "Component.h"
#include "NetCommon.h"

namespace dae
{
	class NetworkHandler :	public Component, olc::net::client_interface<GameMsg>
	{
	public:
		NetworkHandler(GameObject*);
		~NetworkHandler() override;

		virtual void Update(float) override;
		virtual void Start() override;

		void SetNetworkDetails(const std::string& ipv4, uint16_t port) { m_ipv4 = ipv4; m_port = port; }
		void ConnectToServer();
	private:
		void HandleIncomingMessages();
		void HandleOutGoingMessages();

		std::string m_ipv4{"127.0.0.1"};
		uint16_t m_port{5522};

		std::unordered_map<uint32_t, sPlayerDescription> mapObjects;
		uint32_t nPlayerID = 0;

		GameObject* m_player = nullptr;

		bool bWaitingForConnection = true;
	};
}

