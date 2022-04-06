#pragma once
#include "Command.h"

namespace dae
{
	class Comd_TestCommand : public dae::Command
	{
	public:
		void Invoke(dae::GameObject*) override;
	};
}

