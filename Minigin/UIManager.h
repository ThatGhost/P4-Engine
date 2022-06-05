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
		void ClearUI() { m_UIElements.clear(); }
		void UpdateUI();

		UIElement* AddTextElement(std::string*, float, const glm::vec2& position, SDL_Color c = SDL_Color(255,255,255));
		UIElement* AddImageElement(Texture2D**, const glm::vec2& position, const glm::vec2& dimensions);
		UIElement* AddContainer(const glm::vec2& position);
	private:
		friend class Singleton<UIManager>;
		UIManager() = default;

		std::vector<std::unique_ptr<UIElement>> m_UIElements;
	};

	class UIElement
	{
	public:
		UIElement(glm::vec2 pos, bool active = true) :m_Position{ pos }, m_IsActive{ active }{}
		virtual ~UIElement() = default;
		virtual void Render() const = 0;
		virtual void Update() = 0;

		void SetActive(bool active = true) { m_IsActive = active; }
		virtual void Move(glm::vec2 distance) { m_Position += distance; };
	protected:
		bool m_IsActive = true;
		glm::vec2 m_Position;
	};

	class TextElement : public UIElement
	{
	public:
		TextElement(std::string* tptr, float scale, const glm::vec2& pos, SDL_Color color = SDL_Color(255, 255, 255));
		~TextElement() override;
		virtual void Render() const override;
		virtual void Update() override;
	private:
		Texture2D* MakeTextTexture(SDL_Color color, std::string* text, Font* font);
		Texture2D* m_Texture = nullptr;
		std::string* m_Textptr = nullptr;
		float m_Scale = 32;
		SDL_Color m_Color;
		dae::Font* m_Font;
	};
	class ImageElement : public UIElement
	{
	public:
		ImageElement(Texture2D** texture, const glm::vec2& dimensions, const glm::vec2& pos)
			: UIElement(pos,true), m_Texture{texture}, m_Dimensions{dimensions} {};
		virtual void Render() const override;
		virtual void Update() override;
	private:
		Texture2D** m_Texture = nullptr;
		glm::vec2 m_Dimensions;
		const int m_BasicTextureSize{32};
	};
	class ContainerElement : public UIElement
	{
	public:
		ContainerElement(const glm::vec2& pos)
			: UIElement(pos,true) {};
		void AddElement(std::shared_ptr<UIElement> element) { m_Elements.push_back(element); };
		std::shared_ptr<UIElement> operator[](int i) noexcept
		{
			if ((int)m_Elements.size() > i)
				return m_Elements[i];
			else
				return nullptr;
		}
		void Update() override;
		void Move(glm::vec2 pos) override
		{
			m_Position += pos;
			for (auto& element : m_Elements)
			{
				element->Move(pos);
			}
		}
		void Render() const override;
	private:
		std::vector<std::shared_ptr<UIElement>> m_Elements;
	};
}

