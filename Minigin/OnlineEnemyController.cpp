#include "MiniginPCH.h"
#if _WIN64
#define ENVIROMENT64

#include "OnlineEnemyController.h"
#include "SceneManager.h"
#include "GameManagerOnline.h"
#include "RenderComponent.h"
#include "Collider.h"
#include "DestroyComponent.h"
#include "SoundManager.h"

dae::OnlineEnemyController::OnlineEnemyController(GameObject* owner) : Component(owner)
{
	GameManagerOnline* o = static_cast<GameManagerOnline*>( SceneManager::GetInstance().FindComponent<GameManagerOnline>());
	o->SetEnemy(this);
	m_Renderer = static_cast<RenderComponent*>(GetOwner()->GetComponent<RenderComponent>());
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
		SceneManager::GetInstance().AddCollider(coll);
		m_Renderer->SetImage("Player.png", true, 1);
	}
	else
	{
		//red
		//interact with player
		Collider* coll = static_cast<Collider*>(GetOwner()->AddComponent<Collider>());
		coll->SetPosition(glm::vec3(7, 3, 0));
		coll->SetDimensions(glm::vec2(18, 27));
		coll->Setlookers(true, false);
		coll->SetTag("ENEMY");
		SceneManager::GetInstance().AddCollider(coll);

		m_Renderer->SetImage("EvilCook.png", true, 1);
	}

}

void dae::OnlineEnemyController::SetState(int state)
{
	m_State = state;
	m_Renderer->SetRow(state);
}

void dae::OnlineEnemyController::ShootPepper()
{
	if (!m_Good)
		return;

	GameObject* go = new GameObject();
	go->SetParent(GetOwner());
	go->SetStatic(false);

	glm::vec3 myPos{ GetOwner()->GetPosition().GetPosition() };
	float xDir = 0;
	switch (m_State)
	{
	case 0: xDir = 0; break;
	case 1: xDir = -1; break;
	case 2: xDir = 1; break;
	default: xDir = 0; break;
	}

	float pepperpos{ xDir * 28 };
	glm::vec3 pos{ pepperpos, 0, 2 };
	//go->SetPosition(myPos);
	go->Move(pos);

	Collider* coll = static_cast<Collider*>(go->AddComponent<Collider>());
	coll->Setlookers(true, false);
	coll->SetPosition(glm::vec3(0, 0, 0));
	coll->SetDimensions(glm::vec2(20, 20));
	coll->SetTag("PEPPER");
	SceneManager::GetInstance().AddCollider(coll);

	DestroyComponent* destroy = static_cast<DestroyComponent*>(go->AddComponent<DestroyComponent>());
	destroy->Init(0.5f);

	RenderComponent* renderer = static_cast<RenderComponent*>(go->AddComponent<RenderComponent>());
	renderer->SetImage("pepper.png");

	//sound
	SoundManager::GetInstance().PlaySound("Shoot.wav");
}
#endif