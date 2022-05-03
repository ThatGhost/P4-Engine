#include "MiniginPCH.h"
#include "Collider.h"
#include "RenderComponent.h"
#include "BurgerHolder.h"
#include "BurgerComponent.h"

dae::BurgerHolder::BurgerHolder(GameObject* Owner) : Component(Owner)
{

}

void dae::BurgerHolder::Init(const std::string& type, std::vector<dae::Collider*>* colliders)
{
	//make gameobject
	GameObject* burger = new GameObject();
	burger->Move(glm::vec3(-16,0,0));
	//add components
	m_burgerPart = static_cast<dae::BurgerComponent*>(burger->AddComponent<dae::BurgerComponent>());

	dae::Collider* coll = static_cast<dae::Collider*>(burger->AddComponent<dae::Collider>());
	coll->SetPosition(glm::vec3(0,16,0));
	coll->SetDimensions(glm::vec2(64,16));
	coll->SetId(3,-1);
	colliders->push_back(coll);

	dae::RenderComponent* ren = static_cast<dae::RenderComponent*>(burger->AddComponent<dae::RenderComponent>());
	if(type == "Top")ren->SetImage("TopBun.png",false,2);
	//ren->SetAnimationDetails();

	burger->SetParent(GetOwner());
}

dae::BurgerHolder::~BurgerHolder()
{

}

void dae::BurgerComponent::Update(float)
{

}

void dae::BurgerHolder::OnCollideNewBurger(float)
{
	if (m_Argument != m_burgerPart->GetId())
	{
		m_NewBurger = true;
	}
}