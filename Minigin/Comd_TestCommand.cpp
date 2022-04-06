#include "MiniginPCH.h"
#include "Comd_TestCommand.h"
#include "EventManager.h"

void dae::Comd_TestCommand::Invoke(dae::GameObject*)
{
	std::cout << "test command invoked\n";
	EventManager::SendEvent("PressedX",0);
}