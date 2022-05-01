#include "MiniginPCH.h"
#include "FpsComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "EventManager.h"
#include <SDL_ttf.h>


dae::FpsComponent::FpsComponent(GameObject* Owner)
	: Component(Owner), m_TextTexture{nullptr}
{
	m_Font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	EventManager::AddEvent("PressedX",std::bind(&dae::FpsComponent::PressedX,this,1));
}

void dae::FpsComponent::PressedX(int)
{
	std::cout << "PRESSED X EVENT \n\n\n\n";
}

dae::FpsComponent::~FpsComponent()
{
	delete m_TextTexture;
	m_TextTexture = nullptr;
	delete m_Font;
	m_Font = nullptr;
}

void dae::FpsComponent::Update(float deltaTime)
{
	const SDL_Color color = { 255,255,255 }; // only white text is supported now
	float fps = 1 / deltaTime;
	const std::string text = std::to_string(fps);

	const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), text.c_str(), color);
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

	m_TextTexture = new Texture2D(texture);
}

void dae::FpsComponent::Render() const
{
	dae::Renderer::GetInstance().RenderTexture(*m_TextTexture, 0, 0);
}