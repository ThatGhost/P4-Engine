#pragma once
#include "Component.h"
#include "EventManager.h"

#if _WIN64
#define ENVIROMENT64
namespace dae
{
	class ContainerElement;
	class Texture2D;
	class UIElement;
	struct ServerDetails;

	class ServerBrowserComponent : public Component
	{
	public:
		ServerBrowserComponent(GameObject* owner) : Component(owner) {
			EventManager::AddEvent("0BUTTON_B", std::bind(&ServerBrowserComponent::GoBack, this));
		};
		~ServerBrowserComponent() override;
		void StartPolling(const std::string& ticketId) { m_TicketId = ticketId; m_Poll = true; }
		void StopPolling() { m_Poll = false; }
		void SwtichText();
		void FoundMatch(const std::string& ip, uint16_t port);

		void Update(float) override;
		void Start() override;
	private:
		const glm::vec2 m_Margin{80,100};

		std::string m_ServerText{"Connecting To"};
		std::string m_ServerText2{"PlayFab"};

		UIElement* m_ServerElement1 = nullptr;
		UIElement* m_ServerElement2 = nullptr;

		std::string m_SearchText{"Searching for"};
		std::string m_SearchText2{"Match"};

		UIElement* m_SearchElement1 = nullptr;
		UIElement* m_SearchElement2 = nullptr;

		bool m_UIDirty = false;
		bool m_UngracefullDisconnect = true;
		bool m_FoundMatch = false;

		//PLAYFAB
		void LoadPlayfab();
		std::string m_TicketId = "NULL";
		bool m_Poll{false};
		float m_PollTimer = 0;
		const float m_PollTime = 6;

		//Match
		std::string m_Ipv4{"NULL"};
		uint16_t m_Port;

		void GoBack();
	};

	struct ServerDetails
	{
		std::string Name = "";
		std::string IpV4 = "";
		uint16_t Port = 0;
	};
}
#endif