#pragma once
#include <cstdint>

enum class GameMsg : uint32_t
{
	Server_GetStatu = 0,
	Server_GetPing = 1,
	Server_HeartBeat = 2,

	Client_Accepted = 3,
	Client_AssignID = 4,
	Client_RegisterWithServer = 5,
	Client_UnregisterWithServer = 6,

	Game_AddPlayer = 7,
	Game_RemovePlayer = 8,
	Game_UpdatePlayer = 9,

	Game_Lose = 10,
	Game_Start = 11,
	Game_Restart = 13,

	Game_Pepper = 14,
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