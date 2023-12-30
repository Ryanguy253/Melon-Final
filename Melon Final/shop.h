#pragma once
#ifndef SHOP_H
#define SHOP_H
#include "raylib.h"
#include "raymath.h"

#define SHOPSIZE 100
//NPC
#define MAXNPC 100
#define NPCDELAY 3
#define NPCLIFE 60
#define NPC_SPEED_MIN -100
#define NPC_SPEED_MAX -200


const int shop_tile_map_width = 10;
const int shop_tile_map_height = 10;


/*int shop_tile_map[100] = {
	1,0,1,0,1,0,1,0,1,0,
	0,2,0,2,0,2,0,2,0,2,
	1,0,1,0,1,0,1,0,1,0,
	0,1,0,1,0,1,0,1,0,1,
	1,0,1,0,1,0,1,0,1,0,
	0,1,0,1,0,1,0,1,0,1,
	1,0,1,0,1,0,1,0,1,0,
	0,1,0,1,0,1,0,1,0,1,
	1,0,1,0,1,0,1,0,1,0,
	0,1,0,1,0,1,0,1,0,1
};*/

typedef struct shop {
public:
	int item_id;
	int price;
	int condition;
	bool enchanted;

}shop;


typedef struct npc {
	bool active;

	Vector2 position;
	Vector2 velocity;
	float creationTime;

}npc;

npc createNPC(Vector2 position_param, Vector2 velocity_param) {
	npc NPC;
	NPC.active = true;
	NPC.position = position_param;
	NPC.velocity = velocity_param;
	NPC.creationTime = GetTime();

	return NPC;
}

void UpdateNPC(npc* npc, float frametime) {
	if (!npc->active) {
		return;
	}

	//despawn
	//10 is npc life
	if (GetTime() > npc->creationTime + NPCLIFE) {

		npc->active = false;
		return;
	}
	npc->position = Vector2Add(npc->position, Vector2Scale(npc->velocity, frametime));
}


#endif