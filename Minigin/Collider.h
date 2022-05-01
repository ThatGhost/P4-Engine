#pragma once
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
		void CheckCollisions();

		void PassEvent(const std::string& eventName) { m_EventName = eventName; }
		void SetDimensions(const glm::vec2& dimension) { m_Dimensions = dimension; }
		void SetPosition(const glm::vec3& position) { m_Position = position; }
		void SetId(int id, int lookat) { m_Id = id; m_LookAtId = lookat; }
		void AddCollider(Collider* other) { m_CollidersToCheck.push_back(other); }
		glm::vec2 GetPosition();
		int GetId() const { return m_Id; }
		int GetLookId() const { return m_LookAtId; }
	private:
		std::string m_EventName;
		glm::vec3 m_Position;
		glm::vec2 m_Dimensions;
		glm::vec3 m_RealPos;
		std::vector<Collider*> m_CollidersToCheck{};
		int m_Id{};
		int m_LookAtId{-1};


		bool m_WasColliding = false;

		std::shared_ptr<Texture2D> m_Image;
		void EnableDebug(float);
		float m_Argument{};
		bool m_DebugEnabled{false};
	};
}

