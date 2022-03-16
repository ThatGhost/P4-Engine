#pragma once

namespace dae
{
	class GameObject;

	class Command
	{
	public:
		virtual void invoke(GameObject*) = 0;
	};
}

