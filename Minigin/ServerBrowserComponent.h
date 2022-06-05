#pragma once
#include "Component.h"
#include "UIManager.h"

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
		ServerBrowserComponent(GameObject* owner) : Component(owner) {};
		~ServerBrowserComponent() override;
		void ConstructContainer(const ServerDetails&);
		void StartPolling(const std::string& ticketId) { m_TicketId = ticketId; m_Poll = true; }
		void StopPolling() { m_Poll = false; }
		void ChangeServerText(const std::string& text) { m_ServerText = text; UIManager::GetInstance().UpdateUI(); }

		void Update(float) override;
		void Start() override;
	private:
		std::vector<ContainerElement*> m_Containers{};
		std::vector<UIElement*> m_Selectors{};
		std::vector<ServerDetails> m_Servers;

		Texture2D* m_ServerBackGround{nullptr};
		Texture2D* m_ServerIcon{nullptr};
		Texture2D* m_ServerSelector{nullptr};

		const float m_TopDistance{70};
		const glm::vec2 m_Margin{60,20};
		const glm::vec2 m_BlockSize{400,70};
		const glm::vec2 m_IconSize{50,50};
		const glm::vec2 m_IconMargin{10,10};

		const std::string m_BasePath{"..\\Data\\UI\\"};
		std::string m_ServerText{"Server Browser"};

		//PLAYFAB
		void LoadPlayfab();
		std::string m_TicketId;
		bool m_Poll{false};
		float m_PollTimer = 0;
		const float m_PollTime = 6;
	};

	struct ServerDetails
	{
		std::string Name = "";
		std::string IpV4 = "";
		uint16_t Port = 0;
	};
}
#endif