#pragma once
#include "raylib.h"


#define MONSTERSPEED 3

typedef struct Goblin {
	Vector2 position;
	Vector2 offSet;
	int id;
	int dir;
	int health = 100, attack;
	double lastAttack, lastDanage;
	bool player_dir_up, player_dir_down, player_dir_left, player_dir_right;
	bool SameRoom;
	bool ShouldMove;
	bool ShouldAttack;
	bool CanAttack;
	bool Attacked;
	bool IsActive;
	Vector2 CharmPos;
	bool IsCharm;
	int charmID;
}Goblin;