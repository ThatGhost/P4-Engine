#pragma once
#include "Component.h"

namespace dae
{
	class Texture2D;
	using byte = unsigned char;
	class TestComponent : public Component
	{
	public:
		TestComponent(GameObject* Owner);
		virtual ~TestComponent() override;

	private:
		void Test(byte*);
		byte* m_Argument;
		std::shared_ptr<Texture2D> m_UIImage;
		std::string m_Sting{"first string"};
	};
	
}


