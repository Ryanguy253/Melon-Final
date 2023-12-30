#pragma once
#include <iostream>
#include "random.h"
#include <raylib.h>
#include <vector>

class Map {
public:
	Map();
	bool isColliding(Vector2 center);
	bool checkTile(int x, int y);
	int room[25][7][7];
	int map[5][5];
	int Vbridge[4][5];
	int Hbridge[5][4];
	Vector2 start;
	void WalkableTile();
	void Branch(int rowNo, int colNo, int mapSize, int numRooms);
	void Initialize();
	void Print();
	void Draw(Texture Tile[3]);
	void Transfer(int mapData[5][5], Vector2 startPos);
private:
	int roomSize = 7;
	int mapSize = 5;
	int roomPxl = 7 * 80;
	int tilePxl = 80;
	int roomOffset = 3 * 80;
	int numRooms;

};