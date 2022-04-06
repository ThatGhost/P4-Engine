#pragma once
#include "Component.h"

namespace dae
{
	class Collider :
		public Component
	{
	public:

		Collider(dae::GameObject* gameobject);
		void CheckCollisions();

		void PassEventEnter(const std::string& eventName) { m_EventNameEnter = eventName; }
		void PassEventExit(const std::string& eventName) { m_EventNameExit = eventName; }
		void SetDimensions(const glm::vec2& dimension) { m_Dimensions = dimension; }
		void SetPosition(const glm::vec3& position) { m_Position = position; }
		void AddCollider(Collider* other) { m_CollidersToCheck.push_back(other); }
		glm::vec2 GetPosition();
	private:
		std::string m_EventNameEnter;
		std::string m_EventNameExit;
		glm::vec3 m_Position;
		glm::vec2 m_Dimensions;
		glm::vec3 m_RealPos;
		std::vector<Collider*> m_CollidersToCheck{};

		bool m_WasColliding = false;
	};
}

