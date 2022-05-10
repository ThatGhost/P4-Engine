#pragma once
#include "Singleton.h"

namespace dae
{
	using byte = unsigned char;
	class Font;
	class Texture2D;
	class UIElement;

	class UIManager : public Singleton<UIManager>
	{
	public:
		~UIManager() override;

		void Init();
		void Render() const;
		//void DeleteUI(int);
		void ClearUI() { m_UIElements.clear(); }
		void UpdateUI();
		int AddTextElement(std::string*, float, const glm::vec2& position, SDL_Color c = SDL_Color(255,255,255));
		int AddImageElement(Texture2D**, const glm::vec2& position, const glm::vec2& dimensions);
		UIElement* getElement(int i) { return m_UIElements[i].get(); }
	private:
		friend class Singleton<UIManager>;
		UIManager() = default;

		std::vector<std::unique_ptr<UIElement>> m_UIElements;

		byte* m_Argument = nullptr;
	};

	class UIElement
	{
	public:
		UIElement() = default;
		virtual ~UIElement() = default;
		virtual void Render() const = 0;
		virtual void Update() = 0;

		void SetActive(bool active = true) { m_IsActive = active; }
	protected:
		Texture2D* MakeTextTexture(SDL_Color color, std::string* text, Font* font);
		bool m_IsActive = true;
	};

	class TextElement : public UIElement
	{
	public:
		TextElement(std::string* tptr, float scale, const glm::vec2& pos, SDL_Color color = SDL_Color(255, 255, 255));
		~TextElement() override;
		virtual void Render() const override;
		virtual void Update() override;
	private:
		Texture2D* m_Texture = nullptr;
		std::string* m_Textptr = nullptr;
		float m_Scale = 32;
		glm::vec2 m_Position;
		SDL_Color m_Color;
		dae::Font* m_Font;
	};
	class ImageElement : public UIElement
	{
	public:
		ImageElement(Texture2D** texture, const glm::vec2& dimensions, const glm::vec2& pos)
			: m_Texture{ texture }, m_dimensions{ dimensions }, m_Position{ pos } {};
		virtual void Render() const override;
		virtual void Update() override;
	private:
		Texture2D** m_Texture = nullptr;
		glm::vec2 m_Position;
		glm::vec2 m_dimensions;
		const int m_BasicTextureSize{32};
	};
}

