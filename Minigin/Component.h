#pragma once
#include "GameObject.h"

namespace dae {
	class GameObject;

	class Component
	{
	public:
		virtual void Init() {};
		virtual void Update(float) {};
		virtual void Render() const {};

		Component(GameObject* Owner);
		virtual ~Component() {};

		GameObject* GetOwner() const;

	private:
		GameObject* m_Owner;
	};
}

