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
{}

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
		m_UIElements[i].get()->Update();
	}
}

dae::Texture2D* dae::UIElement::MakeTextTexture(SDL_Color color, std::string* text, Font* font)
{
	const auto surf = TTF_RenderText_Blended(font->GetFont(), text->c_str(), color);
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

dae::UIElement* dae::UIManager::AddTextElement(std::string* sptr,float size, const glm::vec2& position, SDL_Color color)
{
	m_UIElements.push_back(std::make_unique<TextElement>(sptr,size,position, color));
	return m_UIElements[m_UIElements.size()-1].get();
}

dae::UIElement* dae::UIManager::AddImageElement(Texture2D** texture, const glm::vec2& dim, const glm::vec2& pos)
{
	m_UIElements.push_back(std::make_unique<ImageElement>(texture,dim,pos));
	return m_UIElements[m_UIElements.size() - 1].get();
}

dae::TextElement::TextElement(std::string* tptr, float scale, const glm::vec2& pos, SDL_Color color)
	:m_Textptr{ tptr }, m_Scale{ scale }, m_Position{ pos }, m_Color{ color }
{
	m_Font = ResourceManager::GetInstance().LoadFont("..\\Data\\Gameplay.ttf",(unsigned int)m_Scale);
}

dae::TextElement::~TextElement()
{
	delete m_Texture;
	delete m_Font;
}

void dae::TextElement::Render() const
{
	if(m_IsActive)
		Renderer::GetInstance().RenderTexture(*m_Texture, m_Position.x, m_Position.y);
}

void dae::TextElement::Update()
{
	delete m_Texture;
	const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Textptr->c_str(), m_Color);
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
	m_Texture = new Texture2D(texture);
}

void dae::ImageElement::Render() const
{
	if (m_IsActive)
		Renderer::GetInstance().RenderTexture(**m_Texture, m_Position.x, m_Position.y, m_dimensions.x, m_dimensions.y);
}

void dae::ImageElement::Update()
{

}
