#pragma once
#ifndef TRADING_H
#define TRADING_H

#include <string>
#include <iostream>
#include<stdio.h>
#include "raylib.h"
#include"shop.h"
using namespace std;

bool npc_said_first_sentence = false;
bool npc_said_second_sentence = false;

int item_id;
/*
_itemsArray[0] = { 0 };
_itemsArray[1] = aurora_veil;
_itemsArray[2] = soulweave_locket;
_itemsArray[3] = timekeepers_hourglass;
_itemsArray[4] = dreamweaver_dreamcatcher;
_itemsArray[5] = vitalis_pendant;
_itemsArray[6] = quicksilver_sylph_amulet;
_itemsArray[7] = oceanic_whisper_conch;
_itemsArray[8] = metamorphos_bloom_cocoon;
_itemsArray[9] = celestial_wayfarer_compass;
_itemsArray[10] = charm_of_wisdom;
*/

const char* getDialog(int dialogue) {
	switch (dialogue) {
	case 10:
		//Charm of wisdom (+charisma) 
		return "Ghost : I crave the wisdom of the ancients, an Echoing Echo Amulet.";
		break;
	case 1:
		//aurora Veil
		return "Ghost : I desire the protection of an Aurora Veil.";
		break;
		// soulweave locket
	case 2:
		return "Ghost : I seek a locket, a Soulweave perhaps.";
		break;
		// timekeeper's hourglass
	case 3:
		return "Ghost : I covet a Transcendent Timekeeper Hourglass.";
		break;
	case 4:
		//Dreamweaver Dreamcatcher
		return "Ghost : I yearn for a Dreamcatcher to weave dreams of futures.";
		break;
	case 5:
		//Vitalis Essence Pendant:
		return "Ghost : I seek the Vitalis Essence Pendant.";
		break;
	case 6:
		//Quicksilver Sylph Amulet:
		return "Ghost : I yearn for the Quicksilver Sylph.";
		break;
	case 7:
		//Oceanic Whisper Conch:
		return "Ghost : The Whisper Conch beckons to me.";
		break;
	case 8:
		//Metamorphos Bloom Cocoon:
		return "Ghost : I yearn for the Bloom Cocoon .";
		break;
	case 9:
		//Celestial Wayfarer Compass:
		return "Ghost : I yearn for the Wayfarer Compass that points towards destiny.";
		break;
	}
}

const char* getName(int ID) {
	switch (ID) {
	case 1:
		return "Aurora Veil";
		break;
	case 2:
		return "Soulweave Locket";
		break;
	case 3:
		return "Timekeeper's Hourglass";
		break;
	case 4:
		return "Dreamweaver Dreamcatcher";
		break;
	case 5:
		return "Vitalis Pendant";
		break;
	case 6:
		return "Quicksilver Sylph Amulet";
		break;
	case 7:
		return "Oceanic Whisper Conch";
		break;
	case 8:
		return "Metamorphosis Bloom Cocoon";
		break;
	case 9:
		return "Celestial Wayfarer Compass";
		break;
	case 10:
		return "Charm Of Wisdom";
		break;
	}
}
//initialise itemsArray
/*
_itemsArray[0] = { 0 };
_itemsArray[1] = aurora_veil;
_itemsArray[2] = soulweave_locket;
_itemsArray[3] = timekeepers_hourglass;
_itemsArray[4] = dreamweaver_dreamcatcher;
_itemsArray[5] = vitalis_pendant;
_itemsArray[6] = quicksilver_sylph_amulet;
_itemsArray[7] = oceanic_whisper_conch;
_itemsArray[8] = metamorphos_bloom_cocoon;
_itemsArray[9] = celestial_wayfarer_compass;
_itemsArray[10] = charm_of_wisdom;
*/






#endif // !TRADING_H