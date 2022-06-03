#include "MiniginPCH.h"
#include "ServerBrowserComponent.h"
#include "UIManager.h"
#include "ResourceManager.h"

dae::ServerBrowserComponent::~ServerBrowserComponent()
{

}

void dae::ServerBrowserComponent::Update(float)
{
	for (auto& e : m_Containers)
	{
		e->Move(glm::vec2(0,10));
	}
}

void dae::ServerBrowserComponent::Start()
{
	//Load textures
	m_ServerBackGround = ResourceManager::GetInstance().LoadTexture(m_BasePath+"ServerBg.png");
	m_ServerIcon = ResourceManager::GetInstance().LoadTexture(m_BasePath+"ServerIcon.png");
	m_ServerSelector = ResourceManager::GetInstance().LoadTexture(m_BasePath+"Arrow.png");

	//construct UI
	UIManager::GetInstance().AddTextElement(&m_ServerText, 32, glm::vec2(m_Margin.x, 10));

	for (size_t i = 0; i < 3; i++)
	{
		ServerDetails d{};
		d.Name = "server" + i;

		m_Servers.push_back(d);
		ConstructContainer(&d);
	}

	UIManager::GetInstance().UpdateUI();
}

void dae::ServerBrowserComponent::ConstructContainer(ServerDetails* details)
{
	ContainerElement* container = static_cast<ContainerElement*>(UIManager::GetInstance().AddContainer(glm::vec2()));

	glm::vec2 pos{};
	pos.x = m_Margin.x;
	pos.y = m_TopDistance + (m_BlockSize.y + m_Margin.y) * m_Servers.size();

	m_Selectors.push_back(UIManager::GetInstance().AddImageElement(&m_ServerSelector,glm::vec2(32,32), glm::vec2(m_Margin.x - 32, pos.y + (m_BlockSize.y/2-16))));
	if (m_Selectors.size() > 1)
	{
		m_Selectors[m_Selectors.size() - 1]->SetActive(false);
	}

	container->AddElement(std::make_shared<ImageElement>(&m_ServerBackGround,m_BlockSize,pos));
	container->AddElement(std::make_shared<ImageElement>(&m_ServerIcon,m_IconSize, pos + m_IconMargin));
	container->AddElement(std::make_shared<TextElement>(&details->Name,(float)25,pos + glm::vec2(80,25),SDL_Color(70,220,70)));
	//container->AddElement(std::make_shared<TextElement>(&m_ServerText,(float)25,pos + glm::vec2(80,25),SDL_Color(70,220,70)));
}
