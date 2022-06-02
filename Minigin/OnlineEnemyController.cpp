#include "MiniginPCH.h"
#include "OnlineEnemyController.h"
#include "SceneManager.h"
#include "GameManagerOnline.h"
#include "RenderComponent.h"
#include "Collider.h"

dae::OnlineEnemyController::OnlineEnemyController(GameObject* owner) : Component(owner)
{
	GameManagerOnline* o = static_cast<GameManagerOnline*>( SceneManager::GetInstance().FindComponent<GameManagerOnline>());
	o->SetEnemy(this);
	m_renderer = static_cast<RenderComponent*>(GetOwner()->GetComponent<RenderComponent>());
}

void dae::OnlineEnemyController::SetGood(bool good)
{
	m_Good = good;
	if (m_Good)
	{
		//white
		//interact with burgers
		Collider* coll = static_cast<Collider*>(GetOwner()->AddComponent<Collider>());
		coll->SetPosition(glm::vec3(7, 3, 0));
		coll->SetDimensions(glm::vec2(18, 27));
		coll->Setlookers(true, false);
		coll->SetTag("PLAYER");

		m_renderer->SetImage("Player.png", true, 1);
	}
	else
	{
		//red
		//interact with player
		Collider* coll = static_cast<Collider*>(GetOwner()->AddComponent<Collider>());
		coll->SetPosition(glm::vec3(7, 3, 0));
		coll->SetDimensions(glm::vec2(18, 27));
		coll->Setlookers(true, false);
		coll->SetTag("ENEMYPLAYER");

		m_renderer->SetImage("EvilCook.png", true, 1);
	}

}
