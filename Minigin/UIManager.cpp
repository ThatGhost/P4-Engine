#include "MiniginPCH.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "EventManager.h"
#include <SDL_ttf.h>

dae::UIManager::~UIManager()
{}

void dae::UIManager::Init()
{
	m_Font = std::make_shared<Font>("..\\Data\\Gameplay.ttf",20);
}

void dae::UIManager::Render() const
{
	for (size_t i = 0; i < m_UIElements.size(); i++)
	{
		m_UIElements[i].get()->Render();
	}
}

void dae::UIManager::UpdateUI()
{
	for (size_t i = 0; i < m_UIElements.size(); i++)
	{
		if (m_UIElements[i].get()->getTextptr() != nullptr)
		{
			const SDL_Color color = { 255,255,255 }; // only white text is supported now
			delete m_UIElements[i].get()->getTextTexture();
			m_UIElements[i].get()->setTextTexture(MakeTextTexture(color, m_UIElements[i].get()->getTextptr()));
		}
	}
}

dae::Texture2D* dae::UIManager::MakeTextTexture(SDL_Color color, std::string* textt)
{
	if (m_Font == nullptr)
		return nullptr;

	const std::string text = *textt;

	const auto surf = TTF_RenderText_Blended(m_Font.get()->GetFont(), text.c_str(), color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);

	return new Texture2D(texture);
}

//void dae::UIManager::DeleteUI(int index)
//{
//	m_UIElements.erase(m_UIElements.begin()+index);
//}

int dae::UIManager::AddTextElement(std::string* sptr, float scale, glm::vec2 pos)
{
	m_UIElements.push_back(std::make_unique<UIElement>(sptr,scale,pos));
	return m_UIElements.size()-1;
}

int dae::UIManager::AddImageElement(std::shared_ptr<Texture2D>* texture, float scale, glm::vec2 pos)
{
	m_UIElements.push_back(std::make_unique<UIElement>(
		texture
		,scale
		,pos));
	return m_UIElements.size() - 1;
}

void dae::UIElement::Render() const
{
	if (!m_IsActive)
		return;

	if (m_Textptr == nullptr)
	{
		Renderer::GetInstance().RenderTexture(**m_Texture, m_Position.x, m_Position.y);
	}
	else
	{
		Renderer::GetInstance().RenderTexture(*m_TextTexture, m_Position.x, m_Position.y);
	}
}

dae::UIElement::~UIElement()
{
	if (m_TextTexture != nullptr)
		delete m_TextTexture;
}