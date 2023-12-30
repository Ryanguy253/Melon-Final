#pragma once
#include "raylib.h"


#define PLAYERSPEED 6

typedef struct Player {

	Vector2 position;
	int dir;
	int health, attack;
	bool player_dir_up, player_dir_down, player_dir_left, player_dir_right;
	bool isPlayerMoving;
	bool isPlayerAttacking;
	bool CanAttack = false;
	double lastAttack, lastDanage;
	int coins;

}Player;

typedef struct Inventory {

	int ITEM1 = 0;
	int ITEM2 = 0;
	int ITEM3 = 0;
	int ITEM4 = 0;

}Inventory;



//attack and attacking