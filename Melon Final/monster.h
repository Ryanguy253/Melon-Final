#pragma once
#include <raylib.h>
#include "map.h"
#include "goblin.h"


class Monster {
public:
	Monster();
	bool EventTriggered(double interval, double* lastInstance);
	void ShouldMove(int x, int y, Goblin goblin[15]);
	void ShouldAttack(int x, int y, Goblin goblin[15]);
	void IsSameRoom(int x, int y, Goblin goblin[15]);
	void Spawn(int map[5][5], Vector2 start, Goblin goblin[15], int* total);
	void Draw(Goblin goblin[15], int id);
	void Move(Goblin goblin[15], int id, int playerX, int playerY);
	int Attack(Goblin goblin[15], int id, int playerX, int playerY);
	void playerAttack(Goblin goblin[15], int damage, int id);
	void DrawHitBox(Goblin goblin[15], int id);
	void IsMonsterDead(Goblin goblin[15]);
private:
	int maxMons;
	int totalMons;
	int totalCharm;
	int roomSize = 7;
	int mapSize = 5;
	int roomPxl = 7 * 80;
	int roomOffset = 3 * 80;
	int tilePxl = 80;
};