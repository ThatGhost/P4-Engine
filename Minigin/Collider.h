#pragma once
#include <functional>
#include <map>
#include "Component.h"

namespace dae
{
	class Texture2D;
	class Collider : public Component
	{
	public:

		Collider(dae::GameObject* gameobject);
		~Collider() override;
#if defined _DEBUG
		virtual void Render() const override;
		virtual void Update(float) override;
#endif
		void CheckCollisions(std::vector<Collider*>* colliders);

		void SetDimensions(const glm::vec2& dimension) { m_Dimensions = dimension; }
		void SetPosition(const glm::vec3& position) { m_Position = position; }
		void SetTag(const std::string& tag) { m_Tag = tag; }
		void Setlookers(bool seen, bool look) { m_CanBeSeen = seen; m_IsLooking = look; }

		glm::vec2 GetPosition();
		std::string GetTag() const { return m_Tag; }
		bool GetSeen() const { return m_CanBeSeen; }
		glm::vec2 GetDimensions() const { return m_Dimensions; }
	private:
		glm::vec3 m_Position;
		glm::vec2 m_Dimensions;
		glm::vec3 m_RealPos;

		std::map<Collider*, bool> m_WasColliding{};
		bool m_CanBeSeen{false};
		bool m_IsLooking{ false };

		std::string m_Tag{"NULL"};

#if defined _DEBUG
		//debug
		Texture2D* m_Image;
		void EnableDebug();
		bool m_DebugEnabled{ false };
#endif
	};
}

