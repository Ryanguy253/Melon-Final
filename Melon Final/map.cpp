#include "map.h"
#include <vector>
#include <raylib.h>


Map::Map() {
	std::cout << "Map()" << std::endl;
	numRooms = 8;
	for (int row = 0; row < mapSize; row++) {
		for (int col = 0; col < mapSize; col++) {
			map[row][col] = 0;
		}
	}
	for (int row = 0; row < 5; row++) {
		for (int col = 0; col < 4; col++) {
			Hbridge[row][col] = 0;

		}
	}
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 5; col++) {
			Vbridge[row][col] = 0;
		}
	}
	for (int i = 0; i < mapSize * mapSize; i++) {
		for (int j = 0; j < roomSize; j++) {
			for (int k = 0; k < roomSize; k++) {
				room[i][j][k] = 0;
			}
		}
	}
	start = { 0,0 };
	Initialize();
}

bool Map::isColliding(Vector2 center) {
	//std::cout << "MapisColliding()" << std::endl;
	int tileX = (int(center.x) % roomPxl) / tilePxl;
	int tileY = (int(center.y) % roomPxl) / tilePxl;
	int roomID, change;


	change = (((int)center.y - 25) % roomPxl) / tilePxl;
	roomID = int((center.y - 25) / roomPxl) * 5 + int(center.x / roomPxl);
	if (room[roomID][change][tileX]) return true;


	change = (((int)center.y + 25) % roomPxl) / tilePxl;
	roomID = int((center.y + 25) / roomPxl) * 5 + int(center.x / roomPxl);
	if (room[roomID][change][tileX]) return true;


	change = (((int)center.x - 35) % roomPxl) / tilePxl;
	roomID = int(center.y / roomPxl) * 5 + int((center.x - 35) / roomPxl);
	if (room[roomID][tileY][change]) return true;


	change = (((int)center.x + 15) % roomPxl) / tilePxl;
	roomID = int(center.y / roomPxl) * 5 + int((center.x + 15) / roomPxl);
	if (room[roomID][tileY][change]) return true;
	return false;
}

bool Map::checkTile(int x, int y) {
	//std::cout << "MapcheckTile()" << std::endl;
	int playerX, playerY;
	Vector2 center;
	playerX = x + roomPxl * start.y + roomOffset; //Player position on map
	playerY = y + roomPxl * start.x + roomOffset;
	center = { (float)playerX + 50,(float)playerY + 50 };
	bool collision = isColliding(center);
	return !collision;
}

void Map::WalkableTile() {
	//std::cout << "MapWalkableTile()" << std::endl;
	for (int row = 0; row < mapSize; row++) {
		for (int col = 0; col < mapSize; col++) {
			int roomID = (5 * row) + col;
			if (map[row][col] == 0) continue;
			for (int rmRow = 0; rmRow < roomSize; rmRow++) {
				for (int rmCol = 0; rmCol < roomSize; rmCol++) {
					//if (rmRow == roomSize - 1 || rmRow == 0 || rmCol == roomSize - 1 || rmCol == 0) { //Check walls
					//	room[roomID][rmRow][rmCol] = 1;	//Set non-walkable tile
					//}
					//else room[roomID][rmRow][rmCol] = 0;
					room[roomID][rmRow][rmCol] = (rmRow == roomSize - 1 || rmRow == 0 || rmCol == roomSize - 1 || rmCol == 0) ? 1 : 0;
				}
			}
			//VBridge 4 x 5
			if (row && Vbridge[row - 1][col] == 1) room[roomID][0][3] = 0;	//Top
			if (Vbridge[row][col]) room[roomID][6][3] = 0;					//Bottom
			//HBridge 5 x 4
			if (col && Hbridge[row][col - 1] == 1) room[roomID][3][0] = 0;	//Left
			if (Hbridge[row][col]) room[roomID][3][6] = 0;					//Right
		}
	}
}

void Map::Branch(int rowNo, int colNo, int mapSize, int numRooms) {
	std::cout << "MapBranch()" << std::endl;
	int i = 1;
	int row, col;
	row = rowNo;
	col = colNo;
	while (i < numRooms)
	{
		int choice = randomNum(0, 3);
		switch (choice)
		{
		case 0:
			col++;
			break;
		case 1:
			col--;
			break;
		case 2:
			row++;
			break;
		case 3:
			row--;
			break;
		}
		if (col < 0 || col > mapSize - 1 || row <0 || row > mapSize - 1) { //Checking if in 5 by 5
			row = rowNo;
			col = colNo;
			continue;
		}
		if (!map[row][col]) { //If grid is empty add room
			map[row][col] = 1;
			switch (choice)
			{
			case 0:
				if (col) Hbridge[row][col - 1] = 1;
				break;
			case 1:
				Hbridge[row][col] = 1;
				break;
			case 2:
				if (row) Vbridge[row - 1][col] = 1;
				break;
			case 3:
				Vbridge[row][col] = 1;
				break;
			}
			i++;
		}
	}
}

void Map::Initialize() {
	std::cout << "MapInitialize()" << std::endl;
	int x = 0;
	int y = 0;
	start = { (float)x, (float)y };
	map[y][x] = 2;

	Branch(y, x, mapSize, numRooms);
	WalkableTile();
}

void Map::Print() {
	//std::cout << "MapPrint()" << std::endl;
	for (int row = 0; row < mapSize; row++) {
		for (int col = 0; col < mapSize; col++) {
			std::cout << map[row][col] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 5; col++) {
			std::cout << Vbridge[row][col] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for (int row = 0; row < 5; row++) {
		for (int col = 0; col < 4; col++) {
			std::cout << Hbridge[row][col] << " ";
		}
		std::cout << std::endl;
	}

}

void Map::Draw(Texture Tile[3]) {
	//std::cout << "MapDraw()" << std::endl;
	ClearBackground({ 37,19,26,255 });
	for (int rowMap = 0; rowMap < mapSize; rowMap++) {
		for (int colMap = 0; colMap < mapSize; colMap++) {
			if (map[rowMap][colMap]) {
				int offsetX = roomPxl * (colMap - (int)start.x) - roomOffset;	//Room anchor player to starting room 
				int offsetY = roomPxl * (rowMap - (int)start.y) - roomOffset;
				DrawTexturePro(Tile[0], { 0,0,112,112 }, { 0,0,(float)roomPxl,(float)roomPxl }, { (float)-offsetX,(float)-offsetY }, 0, WHITE);
				int roomID = rowMap * 5 + colMap;
			}
		}
	}
	for (int row = 0; row < mapSize; row++) {
		for (int col = 0; col < mapSize; col++) {
			if (col < mapSize - 1) {
				if (Hbridge[row][col]) {
					int offsetX = roomPxl * (col - (int)start.y + 1) - 2 * tilePxl - roomOffset;	//Room right -2 tiles
					int offsetY = roomPxl * (row - (int)start.y) + 2 * tilePxl - roomOffset;		//Room top +2 tiles
					DrawTexturePro(Tile[1], { 0,0,64,48 }, { 0,0,(float)4 * tilePxl,(float)3 * tilePxl }, { (float)-offsetX,(float)-offsetY }, 0, WHITE);
				}
			}
			if (row < mapSize - 1) {
				if (Vbridge[row][col]) {
					int offsetX = roomPxl * (col - (int)start.y) + 2 * tilePxl - roomOffset;		//Room left +4 tiles
					int offsetY = roomPxl * (row - (int)start.y + 1) - 2 * tilePxl - roomOffset;	//Room bottom +2 tiles
					DrawTexturePro(Tile[2], { 0,0,48,64 }, { 0,0,(float)3 * tilePxl,(float)4 * tilePxl }, { (float)-offsetX,(float)-offsetY }, 0, WHITE);
				}
			}
		}
	}

}

void Map::Transfer(int mapData[5][5], Vector2 startPos) {
	std::cout << "MapTransfer()" << std::endl;
	for (int row = 0; row < mapSize; row++) {
		for (int col = 0; col < mapSize; col++) {
			mapData[row][col] = map[row][col];
		}
	}
	startPos = Vector2{ start.x,start.y };
}
