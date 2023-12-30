#pragma once
#include <raylib.h>
#include "player.h"

class Dungeon {
public:
	Dungeon();
	bool EventTriggered(int interval, double* lastInstance);
	bool IsPlayerInMap(int x, int y);
	void Initialize();
	void Print();
	void Draw(Texture Tile[3], Player _player);
	void Load(Texture Tile[3]);
	void Update(int x, int y, Player _player, int* health, Player* pointer);
	void PlayerCanAttack(Player _player, Player* pointer);
	void PlayerAttack(Player _player, Player* pointer);
private:
	int roomSize = 7;
	int mapSize = 5;
	int roomPxl = 7 * 80;
	int tilePxl = 80;
	int roomOffset = 3 * 80;
	int maxMons = 0;
};