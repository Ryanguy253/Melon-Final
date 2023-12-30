#include "Dungeon.h"
#include "map.h"
#include <string>
#include "goblin.h"
#include "goblin.h"
#include "monster.h"
#include "map.h"
#include "player.h"


Goblin goblin[15];
Monster enemy;
Map map1;
int map[5][5];
Vector2 start;
int* p;


Dungeon::Dungeon() {
	std::cout << "Dungeon()" << std::endl;
	for (int i = 0; i < mapSize; i++) {
		for (int j = 0; j < mapSize; j++) {
			map[i][j] = 0;
		}
	}
	start = { 0,0 };
	p = &maxMons;
}

bool Dungeon::EventTriggered(int interval, double* lastInstance)
{
	double currenTime = GetTime();
	if (currenTime - *lastInstance >= interval) {
		//std::cout << "CanAttack" << std::endl;
		*lastInstance = currenTime;
		return true;
	}
	return false;

}

bool Dungeon::IsPlayerInMap(int x, int y) {
	//std::cout << "DungeonIsPlayerInMap()" << std::endl;
	return map1.checkTile(x, y);
}

void Dungeon::Initialize() {
	std::cout << "DungeonInitialize()" << std::endl;
	map1.Transfer(map, start);
	enemy = Monster();
	enemy.Spawn(map, start, goblin, p);
	//std::cout << maxMons << std::endl;
}

void Dungeon::Print() {
	std::cout << "DungeonPrint()" << std::endl;
	//map1.Print();
	std::cout << "Dungeon map" << std::endl;
	for (int row = 0; row < mapSize; row++) {
		for (int col = 0; col < mapSize; col++) {
			std::cout << map[row][col] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void Dungeon::Draw(Texture Tile[3], Player _player) {
	//std::cout << "DungeonDraw()" << std::endl;

	map1.Draw(Tile);
	//Charm Drawing
	//Enemy Drawing
	for (int id = 0; id < 15; id++) {
		enemy.Draw(goblin, id);
	}


	if (_player.isPlayerAttacking && _player.CanAttack) {
		DrawCircle(_player.position.x + 40, _player.position.y + 50, 60, WHITE);
	}
	//HitBox
	//DrawCircle(_player.position.x + 40, _player.position.y + 50, 30, RED);
	//for (int id = 0; id < maxMons; id++) {
	//	enemy.DrawHitBox(goblin, id);
	//}



}

void Dungeon::Load(Texture Tile[3]) {
	std::cout << "DungeonLoad()" << std::endl;
	for (int i = 0; i < 3; i++) {
		std::string str = std::to_string(i);
		Tile[i] = LoadTexture(("assets/tiles/tile" + str + ".png").c_str());
	}
}

void Dungeon::Update(int x, int y, Player _player, int* health, Player* pointer) {
	//std::cout << "DungeonUpdate()" << std::endl;
	//enemy.IsSameRoom(x,y,goblin);

	//Monster update
	//static int count1;
	//if (!(count1++ % 100)) {
	//	std::cout << "maxMons" << " ";
	//	std::cout << maxMons << std::endl;
	//}
	//enemy.IsMonsterDead(goblin,p);
	enemy.ShouldMove(x, y, goblin);
	enemy.ShouldAttack(x, y, goblin);
	for (int id = 0; id < 15; id++) {
		if (goblin[id].IsActive) {
			int _x = goblin[id].position.x;
			int _y = goblin[id].position.y;
			if (goblin[id].ShouldMove) {
				enemy.Move(goblin, id, x, y);
				if (!map1.checkTile(goblin[id].position.x + goblin[id].offSet.x, goblin[id].position.y + goblin[id].offSet.y)) {
					goblin[id].position.x = _x;
					goblin[id].position.y = _y;
				}
			}
			if (goblin[id].ShouldAttack && goblin[id].CanAttack) {
				int damage = enemy.Attack(goblin, id, x, y);
				//std::cout << damage << std::endl;
				*health -= damage;
				//std::cout << _player.health << std::endl;
			}
		}
	}



	//Player update
	//std::cout << _player.CanAttack << std::endl;
	PlayerCanAttack(_player, pointer);
	double* p = &_player.lastAttack;
	if (_player.isPlayerAttacking && _player.CanAttack) {
		PlayerAttack(_player, pointer);
	}

}

void Dungeon::PlayerCanAttack(Player _player, Player* pointer) {
	double* p = &pointer->lastAttack;
	bool* point = &pointer->CanAttack;
	//std::cout << *p << std::endl;
	if (EventTriggered(2, p)) {
		*point = true;
	}

}

void Dungeon::PlayerAttack(Player _player, Player* pointer) {
	//std::cout << "DungeonPlayerAttack()" << std::endl;
	Vector2 playerPos, goblinPos;
	bool* point = &pointer->CanAttack;
	playerPos = { (float)_player.position.x + 40, (float)_player.position.y + 50 };
	for (int id = 0; id < maxMons; id++) {
		if (goblin[id].IsActive) {
			goblinPos = { goblin[id].position.x + goblin[id].offSet.x + 40, goblin[id].position.y + 40 + goblin[id].offSet.y };
			if (CheckCollisionCircles(playerPos, 30, goblinPos, 40)) {
				enemy.playerAttack(goblin, 100, id);
				*point = false;
			}
		}
	}
	enemy.IsMonsterDead(goblin);
}