#pragma once
#include <cstdint>

enum class GameMsg : uint32_t
{
	Server_GetStatus,
	Server_GetPing,

	Client_Accepted,
	Client_AssignID,
	Client_RegisterWithServer,
	Client_UnregisterWithServer,

	Game_AddPlayer,
	Game_RemovePlayer,
	Game_UpdatePlayer,

	Game_Lose,
	Game_Start,
	Game_Stop,
	Game_Restart,
};

enum class PlayerState
{
	Idle = 0,
	Left = 1,
	Right = 2,
	Climbing = 3,
	Dead = 4
};

struct sPlayerDescription
{
	uint32_t nUniqueID = 0;

	float x = 0;
	float y = 0;
	int state = 0;

	bool salt = false;
	bool dead = false;
};