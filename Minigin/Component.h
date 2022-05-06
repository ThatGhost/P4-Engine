#pragma once
#include "GameObject.h"

namespace dae {
	class GameObject;
	class Collider;

	class Component
	{
	public:
		virtual void Init() {};
		virtual void Update(float) {};
		virtual void Render() const {};
		virtual void OnCollision(Collider*,Collider*) {};
		virtual void OnCollisionEnter(Collider*,Collider*) {};
		virtual void OnCollisionExit(Collider*,Collider*) {};

		Component(GameObject* Owner);
		virtual ~Component() {};

		GameObject* GetOwner() const;

	private:
		GameObject* m_Owner;
	};
}

