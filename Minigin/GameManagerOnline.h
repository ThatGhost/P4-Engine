#pragma once
#include "GameManager.h"

namespace dae 
{
	class GameManagerOnline : public GameManager
	{
	public:
		GameManagerOnline(GameObject*);
		~GameManagerOnline() override;

		virtual void Update(float) override;
	private:

	};
}

