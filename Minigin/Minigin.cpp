#include "MiniginPCH.h"
#include "Minigin.h"
#include <thread>
#include <chrono>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"

#include "EventManager.h"
#include "SceneConstructor.h"
#include "SoundManager.h"
#include "UIManager.h"

using namespace std;

void PrintSDLVersion()
{
	SDL_version compiled{};
	SDL_version linked{};

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("We compiled against SDL version %d.%d.%d ...\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("We are linking against SDL version %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);
}

void dae::Minigin::Initialize()
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 Game Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		500,
		580,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame()
{
	srand((unsigned int)time(NULL));
	SoundManager::GetInstance().Init();
	UIManager::GetInstance().Init();

	SceneConstructor::ConstructScene("DontDestroyOnLoad.json", false);
	SceneConstructor::ConstructScene("MainMenu.json");

	UIManager::GetInstance().UpdateUI();
	//InputManager::GetInstance().AddPlayer(gm.get());
	//InputManager::GetInstance().InsertCommand<dae::Comd_TestCommand>(dae::ControllerButton::ButtonX);
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();

	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void CollisionThread()
{
	dae::SceneManager::GetInstance().GetActiveScene()->GetCollisions();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init();

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		input.Init();

		// todo: this update loop could use some work.
		bool doContinue = true;
		auto lastTime = chrono::high_resolution_clock::now();
		while (doContinue)
		{
			const auto start = chrono::high_resolution_clock::now();
			float deltaTime = chrono::duration<float>(start - lastTime).count();

			doContinue = input.ProcessInput();
			EventManager::ClearQueue();
			sceneManager.Update(deltaTime);

			std::thread collisionThread(CollisionThread);
			renderer.Render();
			collisionThread.join();

			const auto sleeptime = start + chrono::milliseconds(MsPerFrame) - chrono::high_resolution_clock::now();
			this_thread::sleep_for(sleeptime);
			lastTime = start;
		}
	}

	Cleanup();
}
