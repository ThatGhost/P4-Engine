#pragma once
#include "Singleton.h"
#include "Texture2D.h"
#include <map>

namespace dae
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init();
		Texture2D* LoadTexture(const std::string& file);
		Font* LoadFont(const std::string& file, unsigned int size) const;
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;

		std::map<std::string, std::unique_ptr<Texture2D>> m_Sprites{};
	};
}
