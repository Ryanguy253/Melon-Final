#include "monster.h"
#include "random.h"
#include "goblin.h"
#include <iostream>
#include <cmath>
#include <raylib.h>

int died = 0;

Monster::Monster() {
	std::cout << "Monster()" << std::endl;
	maxMons = 15;
	totalMons = 0;
	totalCharm = 0;
}

bool Monster::EventTriggered(double interval, double* lastInstance) {
	double currenTime = GetTime();
	if (currenTime - *lastInstance >= interval) {
		*lastInstance = currenTime;
		return true;
	}
	return false;
}

void Monster::ShouldMove(int x, int y, Goblin goblin[15]) {
	//std::cout << "MonsterShouldMove()" << std::endl;
	float diffX, diffY;
	int enemyX, enemyY;
	for (int id = 0; id < 15; id++) {
		enemyX = goblin[id].position.x + goblin[id].offSet.x;
		enemyY = goblin[id].position.y + goblin[id].offSet.y;
		diffX = abs(enemyX - x);
		diffY = abs(enemyY - y);
		if (hypot(diffX, diffY) < 400) goblin[id].ShouldMove = true;
		else  goblin[id].ShouldMove = false;
	}
}

void Monster::ShouldAttack(int x, int y, Goblin goblin[15]) {
	//std::cout << "MonsterShouldAttack()" << std::endl;
	float diffX, diffY;
	int enemyX, enemyY;
	for (int id = 0; id < totalMons; id++) {
		//ShouldAttack
		enemyX = goblin[id].position.x + goblin[id].offSet.x;
		enemyY = goblin[id].position.y + goblin[id].offSet.y;
		diffX = abs(enemyX - x);
		diffY = abs(enemyY - y);
		if (hypot(diffX, diffY) < 40) goblin[id].ShouldAttack = true;
		else  goblin[id].ShouldAttack = false;
		//CanAttack
		double* p = &goblin[id].lastAttack;
		if (EventTriggered(2, p)) {
			goblin[id].CanAttack = true;
		}
	}
}

void Monster::IsSameRoom(int x, int y, Goblin goblin[15]) {
	std::cout << "MonsterIsSameRoom()" << std::endl;
	int roomX = x / roomPxl;
	int roomY = y / roomPxl;
	for (int id = 0; id < totalMons; id++) {
		int gobRoomX = (goblin[id].position.x + goblin[id].offSet.x) / roomPxl;
		int gobRoomY = (goblin[id].position.y + goblin[id].offSet.y) / roomPxl;
		if (roomX == gobRoomX && roomY == gobRoomY) goblin[id].SameRoom = 1;
		else goblin[id].SameRoom = 0;
	}
}

void Monster::Spawn(int map[5][5], Vector2 start, Goblin goblin[15], int* total) {
	std::cout << "MonsterMap()" << std::endl;
	std::cout << start.x << std::endl;
	std::cout << start.y << std::endl;
	for (int row = 0; row < mapSize; row++) {
		for (int col = 0; col < mapSize; col++) {
			if (map[row][col] == 1 && (totalMons < maxMons)) {
				int spawns = GetRandomValue(1, 2);
				while (spawns != 0 && totalMons != maxMons) {
					spawns--;
					int xPos = -2 + GetRandomValue(0, 4);
					int yPos = -2 + GetRandomValue(0, 4);
					goblin[totalMons].offSet = { (float)(col - start.x) * roomPxl + xPos * 80,(float)(row - start.y) * roomPxl + yPos * 80 };
					goblin[totalMons].IsActive = true;
					totalMons++;
					//std::cout << totalMons << std::endl;
				}
				map[row][col] = 2;
			}
		}
	}
	*total = totalMons;
}

void Monster::Draw(Goblin goblin[15], int id) {
	//std::cout << "MonsterDraw()" << std::endl;
	//std::cout << totalMons << std::endl;
	if (id < totalMons) {
		if (goblin[id].IsActive) {
			DrawRectangle(goblin[id].position.x + goblin[id].offSet.x, goblin[id].position.y + goblin[id].offSet.y, 80, 80, BLACK);
			if (goblin[id].Attacked) {
				DrawCircle(goblin[id].position.x + goblin[id].offSet.x + 40, goblin[id].position.y + 40 + goblin[id].offSet.y, 40, WHITE);
				goblin[id].Attacked = false;
			}
		}
		if (goblin[id].IsCharm) {
			DrawRectangle(goblin[id].CharmPos.x, goblin[id].CharmPos.y, 20, 20, BLUE);
		}
	}
}

void Monster::Move(Goblin goblin[15], int id, int playerX, int playerY) {
	//std::cout << "MonsterMove()" << std::endl;
	int x_dir = 0, y_dir = 0;
	if (goblin[id].position.x + goblin[id].offSet.x <= playerX) x_dir = 1;
	else x_dir = -1;
	if (goblin[id].position.y + goblin[id].offSet.y <= playerY) y_dir = 1;
	else y_dir = -1;
	goblin[id].position.x += x_dir * MONSTERSPEED;
	goblin[id].position.y += y_dir * MONSTERSPEED;

}

int Monster::Attack(Goblin goblin[15], int id, int playerX, int playerY) {
	std::cout << "MonsterAttack()" << std::endl;
	Vector2 playerPos, goblinPos;
	playerPos = { (float)playerX + 40, (float)playerY + 50 };
	goblinPos = { goblin[id].position.x + goblin[id].offSet.x + 40, goblin[id].position.y + 40 + goblin[id].offSet.y };
	if (CheckCollisionCircles(playerPos, 30, goblinPos, 40)) {
		goblin[id].CanAttack = false;
		goblin[id].Attacked = true;
		return GetRandomValue(0, 5);
	}
}

void Monster::playerAttack(Goblin goblin[15], int damage, int id) {
	std::cout << "MonsterplayerAttacked()" << std::endl;
	goblin[id].health -= damage;
	//std::cout << goblin[id].health << std::endl;
}


void Monster::DrawHitBox(Goblin goblin[15], int id) {
	DrawCircle(goblin[id].position.x + goblin[id].offSet.x + 40, goblin[id].position.y + 40 + goblin[id].offSet.y, 30, RED);
}

void Monster::IsMonsterDead(Goblin goblin[15]) {
	for (int id = 0; id < 15; id++) {
		if (goblin[id].health <= 0 && goblin[id].IsActive) {
			goblin[id].IsActive = false;
			if (GetRandomValue(0, 29) < 30) {
				//Spawn 
				goblin[id].CharmPos = { goblin[id].position.x + goblin[id].offSet.x + 40, goblin[id].position.y + 40 + goblin[id].offSet.y };
				goblin[id].charmID = GetRandomValue(1, 10);
				goblin[id].IsCharm = true;
			}
		}
	}
}




