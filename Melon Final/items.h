#pragma once
#include "raylib.h"
#include <iostream>
#include <string>

using namespace std;

typedef struct items {
	int id;
	const char* name;
	int base_price;
	Texture2D texture;
}items;
