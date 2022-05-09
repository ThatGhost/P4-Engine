#pragma once
#include <functional>
#include <map>
#include "Component.h"

namespace dae
{
	class Texture2D;
	class Collider :
		public Component
	{
	public:

		Collider(dae::GameObject* gameobject);
		virtual void Render() const override;
		virtual void Update(float) override;
		void CheckCollisions(std::vector<Collider*>* colliders);

		void SetDimensions(const glm::vec2& dimension) { m_Dimensions = dimension; }
		void SetPosition(const glm::vec3& position) { m_Position = position; }
		void SetTag(const std::string& tag) { m_tag = tag; }
		void Setlookers(bool seen, bool look) { m_seen = seen; m_look = look; }

		glm::vec2 GetPosition();
		std::string GetTag() { return m_tag; }
		bool GetSeen() { return m_seen; }
		glm::vec2 GetDimensions() { return m_Dimensions; }
	private:
		glm::vec3 m_Position;
		glm::vec2 m_Dimensions;
		glm::vec3 m_RealPos;

		std::map<Collider*, bool> m_WasColliding{};
		bool m_seen{false};
		bool m_look{ false };

		std::string m_tag{"NULL"};

		//debug
		std::shared_ptr<Texture2D> m_Image;
		void EnableDebug(float);
		float m_Argument{};
		bool m_DebugEnabled{false};
	};
}

