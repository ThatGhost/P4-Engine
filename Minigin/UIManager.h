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
		int AddTextElement(std::string*, float, glm::vec2);
		int AddImageElement(std::shared_ptr<Texture2D>*, float, glm::vec2);
		UIElement* getElement(int i) { return m_UIElements[i].get(); }
	private:
		friend class Singleton<UIManager>;
		UIManager() = default;

		Texture2D* MakeTextTexture(SDL_Color color, std::string* text);

		std::shared_ptr<Font> m_Font;
		std::vector<std::unique_ptr<UIElement>> m_UIElements;

		byte* m_Argument = nullptr;
	};

	class UIElement
	{
	public:
		//i use pointers to avoind unnecesairy fuction calling to update graphics
		UIElement(std::shared_ptr<Texture2D>* texture, float scale, glm::vec2 pos) 
			: m_Texture{ texture }, m_Scale{ scale }, m_Position{ pos } {};

		UIElement(std::string* tptr, float scale, glm::vec2 pos, bool isStatic = true) 
			:m_Textptr{ tptr }, m_Scale{ scale }, m_Position{ pos }, m_IsStatic{isStatic} {}

		~UIElement();
		void Render() const;
		void SetActive(bool active = true) { m_IsActive = active; }
		std::string* getTextptr() { return m_Textptr; }
		std::shared_ptr<Texture2D>* getTexture() { return m_Texture; }
		Texture2D* getTextTexture() { return m_TextTexture; }
		void setTextTexture(Texture2D* t) { m_TextTexture = t; }
	private:

		std::shared_ptr<Texture2D>* m_Texture = nullptr;
		Texture2D* m_TextTexture = nullptr;
		std::string* m_Textptr = nullptr;
		float m_Scale;
		glm::vec2 m_Position;
		bool m_IsStatic = true;
		bool m_IsActive = true;
	};
}

