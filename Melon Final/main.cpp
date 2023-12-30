#include "raylib.h"
#include <stdio.h>
#include "player.h"
#include <iostream>
#include "Dungeon.h"
#include <vector>
#include <string>
#include "shop.h"
#include "raymath.h"
#include "trading.h"
#include "items.h"

using namespace std;

//Edit
//monitor
static int monitor;

//Dungeon 
Dungeon Game1;
Texture Tile[3];

//attack
Texture sword_texture;
Texture player_sprite_slash[4];
Texture sword_sprite_slash[4];
int swordOffX = 0, swordOffY = 0;
bool atRest;
int* pointer;

//player
Player _player;
Player* pointerPlayer;
Texture player_texture;
Texture player_sprite_up;
Texture player_sprite_down;
Texture player_sprite_left;
Texture player_sprite_right;
Rectangle playerSrc = { 0,0,24,24 };
Rectangle playerDest = { 0,0,100,100 };// 10 10 is size
Vector2 playerCenter = { playerSrc.width / 2,playerSrc.height / 2 };

//window
int window_height = 400;
int window_width = 800;

//camera
Camera2D camera;
//Vector2 camera_offset = { float(window_width / 2), float(window_height / 2) };
//Vector2 camera_pos = { float(playerDest.x - (playerDest.width / 2)),float(playerDest.y - (playerDest.height / 2)) };
Vector2 camera_offset = { 0 };
Vector2 camera_pos = { 0 };



//game tick
int player_tick;
int game_tick;

//shop items
#define SHOP_ITEMS 4
items _itemsArray[11];
shop _shop[SHOP_ITEMS] = { 0 };

//npc
npc _npc[MAXNPC] = { 0 };
float _lastnpccreationtime = 1;

//trading game
bool market_scene = false;
// initialise trading scene
Texture2D trading_background;
bool teleport_flag = false;
Texture2D ghost;
bool collide_with_npc = false;
bool trading = false;
npc* npc_that_collide_with_player = NULL;
bool npc_quit_trading = false;
bool trades = 0;
int trade_count = 0;

#define RANDOM_ARRAY_SIZE 200
int random_array[RANDOM_ARRAY_SIZE] = { 0 };
bool item_match = false;
bool item_does_not_match = false;
bool npc_rejection = false;
bool npc_accept = false;
int nego_price = 0;
int npc_acceptable_price_range;
bool closing_deal = false;
bool sucessful_deal = false;
int itemNum = 0;
bool pause_game_dialogue = false;
int pause_x;
int pause_y;
bool first_phase;
bool im_looking_for = false;
bool looking_for_charm_name = false;
bool key_f_pressed = false;
bool nego_phase = false;
bool remove_player_item = false;
bool thats_a_great_deal = false;
bool npc_stop_trading = false;
bool update_money = false;

//inventory
Inventory _player_inventory;
Texture slot1;
Texture slot2;
Texture slot3;
Texture slot4;

//title screen
bool titlescreenshown = false;



/*void ToggleFullScreenWindow(int windowWidth, int windowHeight) {
	if (!IsWindowFullscreen()) {
		int monitor = GetCurrentMonitor();
		SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
		ToggleFullscreen();
	}
	else {
		ToggleFullscreen();
		SetWindowSize(windowWidth, windowHeight);
	}
}*/

void initialise() {
	InitWindow(800, 450, "GAME JAM");
	SetTargetFPS(60);

	//window
	monitor = GetCurrentMonitor();
	int current_monitor_height = GetMonitorHeight(monitor);
	int current_monitor_width = GetMonitorWidth(monitor);

	SetWindowSize(current_monitor_width, current_monitor_height);
	ToggleFullscreen();
	SetTargetFPS(60);


	//initialise player sprite
	player_sprite_up = LoadTexture("assets/character/Walk Up.png");
	player_sprite_down = LoadTexture("assets/character/Walk Down.png");
	player_sprite_left = LoadTexture("assets/character/Walk Left.png");
	player_sprite_right = LoadTexture("assets/character/Walk Right.png");




	//attack sprite
	player_sprite_slash[3] = LoadTexture("assets/character/Slash Right.png");
	sword_sprite_slash[3] = LoadTexture("assets/character/Sword Right.png");
	player_sprite_slash[2] = LoadTexture("assets/character/Slash Down.png");
	sword_sprite_slash[2] = LoadTexture("assets/character/Sword Down.png");
	player_sprite_slash[1] = LoadTexture("assets/character/Slash Up.png");
	sword_sprite_slash[1] = LoadTexture("assets/character/Sword Up.png");
	player_sprite_slash[0] = LoadTexture("assets/character/Slash Left.png");
	sword_sprite_slash[0] = LoadTexture("assets/character/Sword Left.png");

	//initialise camera
	camera = { camera_offset,camera_pos,0,1 };

	//initalise trading
	trading_background = LoadTexture("assets/nightMarket/night_market.png");
	ghost = LoadTexture("assets/nightMarket/ghost.png");

	// initialise random array
	for (int i = 0; i < RANDOM_ARRAY_SIZE; i++) {
		random_array[i] = GetRandomValue(10, 10);
	}
	//initialise items

	items charm_of_wisdom;
	charm_of_wisdom.id = 1;
	charm_of_wisdom.base_price = 10;
	charm_of_wisdom.texture = LoadTexture("assets/Items/charm_of_wisdom.png");
	charm_of_wisdom.name = "Charm Of Wisdom";

	items aurora_veil;
	aurora_veil.id = 2;
	aurora_veil.base_price = 15;
	aurora_veil.texture = LoadTexture("assets/Items/aurora_veil.png");
	aurora_veil.name = "Aurora Veil";

	items soulweave_locket;
	soulweave_locket.id = 3;
	soulweave_locket.base_price = 13;
	soulweave_locket.texture = LoadTexture("assets/Items/soulweave_locket.png");
	soulweave_locket.name = "Soulweave Locket";

	items timekeepers_hourglass;
	timekeepers_hourglass.id = 4;
	timekeepers_hourglass.base_price = 11;
	timekeepers_hourglass.texture = LoadTexture("assets/Items/timekeepers_hourglass.png");
	timekeepers_hourglass.name = "Timekeeper's Hourglass";

	items dreamweaver_dreamcatcher;
	dreamweaver_dreamcatcher.id = 5;
	dreamweaver_dreamcatcher.base_price = 12;
	dreamweaver_dreamcatcher.texture = LoadTexture("assets/Items/dreamweaver_dreamcatcher.png");
	dreamweaver_dreamcatcher.name = "Dreamweaver Dreamcatcher";

	items vitalis_pendant;
	vitalis_pendant.id = 6;
	vitalis_pendant.base_price = 16;
	vitalis_pendant.texture = LoadTexture("assets/Items/vitalis_pendant.png");
	vitalis_pendant.name = "Vitalis Pendant";

	items quicksilver_sylph_amulet;
	quicksilver_sylph_amulet.id = 7;
	quicksilver_sylph_amulet.base_price = 20;
	quicksilver_sylph_amulet.texture = LoadTexture("assets/Items/quicksilver_sylph_amulet.png");
	quicksilver_sylph_amulet.name = "Quicksilver Sylph Amulet";

	items oceanic_whisper_conch;
	oceanic_whisper_conch.id = 8;
	oceanic_whisper_conch.base_price = 25;
	oceanic_whisper_conch.texture = LoadTexture("assets/Items/oceanic_whisper_conch.png");
	oceanic_whisper_conch.name = "Oceanic Whisper Conch";

	items metamorphos_bloom_cocoon;
	metamorphos_bloom_cocoon.id = 9;
	metamorphos_bloom_cocoon.base_price = 23;
	metamorphos_bloom_cocoon.texture = LoadTexture("assets/Items/metamorphos_bloom_cocoon.png");
	metamorphos_bloom_cocoon.name = "Metamorphosis Bloom Cocoon";

	items celestial_wayfarer_compass;
	celestial_wayfarer_compass.id = 10;
	celestial_wayfarer_compass.base_price = 30;
	celestial_wayfarer_compass.texture = LoadTexture("assets/Items/celestial_wayfarer_compass.png");
	celestial_wayfarer_compass.name = "Celestial Wayfarer Compass";


	//initialise itemsArray

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

	//initialise inventory
	_player_inventory.ITEM1 = 0;
	_player_inventory.ITEM2 = 0;
	_player_inventory.ITEM3 = 0;
	_player_inventory.ITEM4 = 0;

}

void dungeoninitialiseloadingtiles() {
	//Loading Tiles
	Game1.Initialize();
	Game1.Load(Tile);
	Game1.Print();
	_player.health = 100;
	pointerPlayer = &_player;
	pointer = &_player.health;
	std::cout << _player.health << std::endl;

}


void togglemarketscene() {
	if (IsKeyPressed(KEY_O)) {
		
		market_scene = !market_scene;
		cout << "Market Scene Toggled" << endl;
		
		if (!market_scene) {
			_player.position.x = -1760;
			_player.position.y = 0;
		}
		else if (market_scene) {
			_player.position.x = 0;
			_player.position.y = 1350;
		}

	}
}

void addNPC(Vector2 position) {
	//position of NPC spawn
	bool created = false;
	Vector2 velocity = Vector2Subtract(Vector2{ -10000,position.y }, position);
	velocity = Vector2Scale(Vector2Normalize(velocity), GetRandomValue(NPC_SPEED_MIN, NPC_SPEED_MAX));

	npc npc = createNPC(position, velocity);
	for (int i = 0; i < MAXNPC; i++) {
		if (_npc[i].active) {
			continue;
		}
		_npc[i] = npc;
		break;
	}
	if (!created) {
		TraceLog(LOG_ERROR, "Failed to create an NPC because there were no inactive spots in the array");
	}

}

Vector2 GetNextNPCPostion() {
	return Vector2{ -100, 1330 };
}

void DrawNpc(npc npc) {

	if (!npc.active) {
		return;
	}

	DrawTexturePro(ghost, { 0,0,16,16 }, { npc.position.x,npc.position.y,100,100 }, { 0,0 }, 0, WHITE);
}

void drawShop() {
	// draw npc
	for (int i = 0; i < MAXNPC; i++) {
		DrawNpc(_npc[i]);
	}
}

void drawInventoryUI() {
	DrawText((TextFormat("Coins : %i", _player.coins)), (playerDest.x + (playerDest.width / 2) + 70 + 80 + 80 + 80), (float(playerDest.y + (playerDest.height / 2)) - GetScreenHeight() / 2 + 40), 30, YELLOW);
	if (_player_inventory.ITEM1 == 10 || _player_inventory.ITEM2 == 10 || _player_inventory.ITEM3 == 10 || _player_inventory.ITEM4 == 10 && market_scene) {
		DrawText("+10% coins each sale (Charm of Wisdom)", (playerDest.x + (playerDest.width / 2) + 70 + 80 + 80 + 80), (float(playerDest.y + (playerDest.height / 2)) - GetScreenHeight() / 2 + 80), 30, YELLOW);
	}

	Rectangle r{ ((playerDest.x + (playerDest.width / 2))), float(playerDest.y + (playerDest.height / 2)) - GetScreenHeight() / 2,400,100 };
	DrawRectanglePro(r, Vector2{ 400 / 2,-50 / 2 }, 0, BROWN);

	Rectangle r1{ ((playerDest.x + (playerDest.width / 2)) + 10), float(playerDest.y + (playerDest.height / 2)) - (GetScreenHeight() / 2) + 30 ,80,80 };
	DrawRectanglePro(r1, Vector2{ 400 / 2,0 }, 0, DARKBROWN);

	Rectangle r2{ ((playerDest.x + (playerDest.width / 2)) + 30 + 80), float(playerDest.y + (playerDest.height / 2)) - (GetScreenHeight() / 2) + 30,80,80 };
	DrawRectanglePro(r2, Vector2{ 400 / 2,0 }, 0, DARKBROWN);

	Rectangle r3{ ((playerDest.x + (playerDest.width / 2)) + 50 + 80 + 80), float(playerDest.y + (playerDest.height / 2)) - (GetScreenHeight() / 2) + 30,80,80 };
	DrawRectanglePro(r3, Vector2{ 400 / 2,0 }, 0, DARKBROWN);

	Rectangle r4{ ((playerDest.x + (playerDest.width / 2)) + 70 + 80 + 80 + 80), float(playerDest.y + (playerDest.height / 2)) - (GetScreenHeight() / 2) + 30,80,80 };
	DrawRectanglePro(r4, Vector2{ 400 / 2,0 }, 0, DARKBROWN);

	//initialise and draw textures for slots only if inventory contains something

	slot1 = _itemsArray[_player_inventory.ITEM1].texture;
	slot2 = _itemsArray[_player_inventory.ITEM2].texture;
	slot3 = _itemsArray[_player_inventory.ITEM3].texture;
	slot4 = _itemsArray[_player_inventory.ITEM4].texture;


	// draw inventory and hover over
	if (_player_inventory.ITEM1 != 0) {
		DrawTexturePro(slot1, { 0,0,16,16 }, r1, Vector2{ 400 / 2,0 }, 0, WHITE);
	}
	if (_player_inventory.ITEM2 != 0) {
		DrawTexturePro(slot2, { 0,0,16,16 }, r2, Vector2{ 400 / 2,0 }, 0, WHITE);
	}
	if (_player_inventory.ITEM3 != 0) {
		DrawTexturePro(slot3, { 0,0,16,16 }, r3, Vector2{ 400 / 2,0 }, 0, WHITE);
	}
	if (_player_inventory.ITEM4 != 0) {
		DrawTexturePro(slot4, { 0,0,16,16 }, r4, Vector2{ 400 / 2,0 }, 0, WHITE);
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		//slot 1
		if (_player_inventory.ITEM1 != 0) {
			DrawText(TextFormat("1 : %s ", getName(_player_inventory.ITEM1)), playerDest.x - 100, playerDest.y - 205, 25, RAYWHITE);
		}
		else if (_player_inventory.ITEM1 = 0) {
			DrawText("1 : EMPTY ", playerDest.x - 100, playerDest.y - 205, 25, RAYWHITE);
		}
		//slot 2
		if (_player_inventory.ITEM2 != 0) {
			DrawText(TextFormat("2 : %s ", getName(_player_inventory.ITEM2)), playerDest.x - 100, playerDest.y - 180, 25, RAYWHITE);
		}
		else if (_player_inventory.ITEM2 = 0) {
			DrawText("2 : EMPTY ", playerDest.x - 100, playerDest.y - 180, 25, RAYWHITE);
		}
		//slot 3
		if (_player_inventory.ITEM3 != 0) {
			DrawText(TextFormat("3 : %s ", getName(_player_inventory.ITEM3)), playerDest.x - 100, playerDest.y - 155, 25, RAYWHITE);
		}
		else if (_player_inventory.ITEM3 = 0) {
			DrawText("3 : EMPTY ", playerDest.x - 100, playerDest.y - 155, 25, RAYWHITE);
		}
		//slot 4
		if (_player_inventory.ITEM4 != 0) {
			DrawText(TextFormat("4 : %s ", getName(_player_inventory.ITEM4)), playerDest.x - 100, playerDest.y - 130, 25, RAYWHITE);
		}
		else if (_player_inventory.ITEM4 = 0) {
			DrawText("4 : EMPTY ", playerDest.x - 100, playerDest.y - 130, 25, RAYWHITE);
		}

	}
}

void trading_game_update() {

	if (GetTime() > _lastnpccreationtime + NPCDELAY) {
		addNPC(GetNextNPCPostion());
		_lastnpccreationtime = GetTime();
	}

	//update npc
	for (int i = 0; i < MAXNPC; i++) {
		UpdateNPC(&_npc[i], GetFrameTime());
	}

	//check for collision with NPC
	for (int i = 0; i < MAXNPC; i++) {
		if (!_npc->active) {
			continue;
		}
		if (CheckCollisionPointRec({ playerDest.x,playerDest.y }, { _npc[i].position.x,_npc[i].position.y,100,100 })) {
			collide_with_npc = true;

			if (market_scene && !im_looking_for && !looking_for_charm_name && !key_f_pressed) {
				DrawText("Press E to interact", playerDest.x - 150, playerDest.y - 100, 35, WHITE);
				if (IsKeyPressed(KEY_E)) {
					im_looking_for = true;
					npc_that_collide_with_player = &_npc[i];
					npc_that_collide_with_player->velocity = { 0 };

					trading = true;
				}
			}
		}
	}

	/*if (IsKeyPressed(KEY_E)) {
		im_looking_for = true;
		npc_quit_trading = false;
		trading = true;
		npc_said_first_sentence = false;
		npc_said_second_sentence = false;
		npc_that_collide_with_player->velocity = { 0 };
	}*/

	if (trading) {
		npc_that_collide_with_player->active = true;
	}
}

void trading_game_render() {
	drawShop();
	drawInventoryUI();


	if (im_looking_for) {
		if (IsKeyDown(KEY_ENTER)) {
			im_looking_for = false;
			looking_for_charm_name = true;
		}
	}

	if (looking_for_charm_name) {
		//change hardcoded values
		if (IsKeyPressed(KEY_F)) {
			key_f_pressed = true;
			trade_count++;
			looking_for_charm_name = false;
		}
	}

	//i dont have it
	if (key_f_pressed) {
		if (IsKeyPressed(KEY_ENTER)) {
			npc_that_collide_with_player->velocity = { 100,-100 };
			key_f_pressed = false;
		}
	}

	//check items if match
	if (IsKeyPressed(KEY_ONE) && looking_for_charm_name) {
		if (_player_inventory.ITEM1 == random_array[trade_count]) {
			nego_phase = true;
		}
		else {
			nego_phase = false;
		}
	}
	if (IsKeyPressed(KEY_TWO) && looking_for_charm_name) {
		if (_player_inventory.ITEM2 == random_array[trade_count]) {
			nego_phase = true;
		}
		else {
			nego_phase = false;
		}
	}
	if (IsKeyPressed(KEY_THREE) && looking_for_charm_name) {
		if (_player_inventory.ITEM3 == random_array[trade_count]) {
			nego_phase = true;
		}
		else {
			nego_phase = false;
		}
	}
	if (IsKeyPressed(KEY_FOUR) && looking_for_charm_name) {
		if (_player_inventory.ITEM4 == random_array[trade_count]) {
			nego_phase = true;
		}
		else {
			nego_phase = false;
		}
	}

	//negotiation phase
	if (IsKeyPressed(KEY_UP) && nego_phase) {
		nego_price++;
	}
	if (IsKeyPressed(KEY_DOWN) && nego_phase) {
		nego_price--;
	}
	if (nego_price < 0) {
		nego_price = 0;
	}

	if (IsKeyPressed(KEY_ENTER) && nego_phase) {
		npc_acceptable_price_range = GetRandomValue(_itemsArray[random_array[trade_count] - 1].base_price - 30, _itemsArray[random_array[trade_count] - 1].base_price + 15);
		cout << "Accepttable price range : " << npc_acceptable_price_range << endl;
		if (nego_price < _itemsArray[random_array[trade_count] - 1].base_price + npc_acceptable_price_range) {
			cout << " Trade Accepted " << endl;
			closing_deal = true;
			nego_phase = false;
			thats_a_great_deal = true;
		}
		else {
			closing_deal = false;
			nego_phase = false;
			thats_a_great_deal = false;
			npc_stop_trading = true;
		}
	}

	if (closing_deal && !nego_phase && thats_a_great_deal) {
		if (IsKeyPressed(KEY_ENTER)) {

			closing_deal = false;
			sucessful_deal = true;
			remove_player_item = true;
			update_money = true;
		}
	}

	//remove player item and add coins
	if (remove_player_item && update_money) {
		_player.coins += nego_price;
		if (_player_inventory.ITEM1 == 10 || _player_inventory.ITEM2 == 10 || _player_inventory.ITEM3 == 10 || _player_inventory.ITEM4 == 10) {
			_player.coins += int(nego_price * 0.1);
		}
		cout << "PLAYER COINS" << _player.coins << endl;
		if (_player_inventory.ITEM1 == random_array[trade_count]) {
			_player_inventory.ITEM1 = 0;
		}
		else if (_player_inventory.ITEM2 == random_array[trade_count]) {
			nego_phase = true;
			_player_inventory.ITEM1 = 0;
		}
		else if (_player_inventory.ITEM3 == random_array[trade_count]) {
			nego_phase = true;
			_player_inventory.ITEM1 = 0;
		}
		else if (_player_inventory.ITEM4 == random_array[trade_count]) {
			nego_phase = true;
			_player_inventory.ITEM1 = 0;
		}
		remove_player_item = false;
		update_money = false;
	}

	if (sucessful_deal) {
		if (IsKeyPressed(KEY_SPACE)) {
			sucessful_deal = false;
			im_looking_for = false;
			looking_for_charm_name = false;
			key_f_pressed = false;
			nego_phase = false;
			remove_player_item = false;
			thats_a_great_deal = false;
			npc_that_collide_with_player->velocity = { 100,-100 };
		}
		if (IsKeyPressed(KEY_ENTER)) {
			update_money = false;
		}
	}

	if (npc_stop_trading) {
		if (IsKeyPressed(KEY_SPACE)) {
			sucessful_deal = false;
			im_looking_for = false;
			looking_for_charm_name = false;
			key_f_pressed = false;
			nego_phase = false;
			remove_player_item = false;
			thats_a_great_deal = false;
			npc_stop_trading = false;
			npc_that_collide_with_player->velocity = { 100,-100 };
		}
	}

}
void trading_dialogue() {
	int temp;
	if (im_looking_for) {
		DrawText("Ghost : I'm looking for a charm that will guide me in my next life", playerDest.x - GetMonitorWidth(monitor) / 4, 1600, 35, WHITE);
		DrawText("Press Enter to continue", playerDest.x - 150, playerDest.y - 100, 35, WHITE);
	}
	if (looking_for_charm_name && !nego_phase && !thats_a_great_deal && !npc_stop_trading) {
		DrawText(getDialog(random_array[trade_count]), playerDest.x - GetMonitorWidth(monitor) / 4, 1600, 35, WHITE);
		DrawText("Use Number Keys 1-4 to offer items", playerDest.x - GetMonitorWidth(monitor) / 4, 1700, 35, RED);
		DrawText("I don't have it... -> Press KEY F (Ghost will leave)", playerDest.x - 150, playerDest.y - 100, 35, WHITE);
	}
	if (key_f_pressed) {
		DrawText("Ghost : I'm sorry, I have to go.", playerDest.x - GetMonitorWidth(monitor) / 4, 1700, 35, WHITE);
		DrawText("Press Enter to continue", playerDest.x - 150, playerDest.y - 100, 35, WHITE);
	}
	if (nego_phase) {
		DrawText("Ghost : Yes, that's it. How much?", playerDest.x - GetMonitorWidth(monitor) / 4, 1700, 35, WHITE);

		DrawText(TextFormat("Price: %i (Use arrow keys to change) (Use ENTER to confirm)", nego_price), playerDest.x - GetMonitorWidth(monitor) / 4, 1800, 30, { 255,126,0,255 });
		DrawText(TextFormat("Base Price: %i ", _itemsArray[random_array[trade_count] - 1].base_price), playerDest.x - GetMonitorWidth(monitor) / 4, 1900, 30, { 255,126,0,255 });
	}
	if (closing_deal && !nego_phase) {
		DrawText("Ghost : Thats a great deal !! I'll take it.", playerDest.x - GetMonitorWidth(monitor) / 4, 1700, 35, WHITE);
		DrawText("Press Enter to continue ", playerDest.x - 150, playerDest.y - 100, 35, WHITE);
	}
	if (sucessful_deal) {
		if (_player_inventory.ITEM1 == 10 || _player_inventory.ITEM2 == 10 || _player_inventory.ITEM3 == 10 || _player_inventory.ITEM4 == 10) {
			temp = nego_price + int(nego_price * 0.1);
		}
		DrawText(TextFormat("You gained %i coins. (Press SPACE to continue) ", temp), playerDest.x - 160, playerDest.y - 100, 35, WHITE);
	}
	if (npc_stop_trading) {
		DrawText("Ghost : I don't have that much...", playerDest.x - GetMonitorWidth(monitor) / 4, 1700, 35, WHITE);
		DrawText("Press SPACE to continue ", playerDest.x - 150, playerDest.y - 100, 35, WHITE);
	}


}

void trading_game() {
	DrawTexture(trading_background, 0, 0, WHITE);
	if (!teleport_flag) {
		_player.position.x = 0;
		_player.position.y = 1350;
		teleport_flag = true;
	}
	//collision with side
	if (_player.position.x < trading_background.width / 3) {
		_player.position.x = trading_background.width / 3;
	}
	if (_player.position.x > trading_background.width - trading_background.width / 2.5) {
		_player.position.x = trading_background.width - trading_background.width / 2.5;
	}

	trading_dialogue();
	trading_game_update();

	trading_game_render();

	//set collide to false
	collide_with_npc = false;
}


void input() {
	
	togglemarketscene();
	if (IsKeyDown(KEY_W)) {
		_player.isPlayerMoving = true;
		_player.player_dir_up = true;
		_player.dir = 1;
	}
	if (IsKeyDown(KEY_S)) {
		_player.isPlayerMoving = true;
		_player.player_dir_down = true;
		_player.dir = 2;
	}
	
	if (IsKeyDown(KEY_A)) {
		_player.isPlayerMoving = true;
		_player.player_dir_left = true;
		_player.dir = 0;
	}
	
	if (IsKeyDown(KEY_D)) {
		_player.isPlayerMoving = true;
		_player.player_dir_right = true;
		_player.dir = 3;
	}
	if (IsKeyDown(KEY_SPACE)) {
		_player.isPlayerAttacking = true;
	}
	else _player.isPlayerAttacking = false;
	//attacking
}

void marketinput() {
	
	togglemarketscene();
		//disable up and down for market
		if (!market_scene) {
			if (IsKeyDown(KEY_W)) {
				_player.isPlayerMoving = true;
				_player.player_dir_up = true;
				_player.dir = 1;
			}
			if (IsKeyDown(KEY_S)) {
				_player.isPlayerMoving = true;
				_player.player_dir_down = true;
				_player.dir = 2;
			}
		}
		if (IsKeyDown(KEY_A)) {
			_player.isPlayerMoving = true;
			_player.player_dir_left = true;
			_player.dir = 0;
		}
		if (IsKeyDown(KEY_D)) {
			_player.isPlayerMoving = true;
			_player.player_dir_right = true;
			_player.dir = 3;
		}
	
}


void update() {

	atRest = false;

	if (_player.isPlayerMoving || _player.isPlayerAttacking) {
		if (_player.player_dir_up) {
			_player.position.y -= PLAYERSPEED;
			if (!Game1.IsPlayerInMap((int)_player.position.x, (int)_player.position.y)) _player.position.y += PLAYERSPEED;
		}
		if (_player.player_dir_down) {
			_player.position.y += PLAYERSPEED;
			if (!Game1.IsPlayerInMap((int)_player.position.x, (int)_player.position.y)) _player.position.y -= PLAYERSPEED;
		}
		if (_player.player_dir_left) {
			_player.position.x -= PLAYERSPEED;
			if (!Game1.IsPlayerInMap((int)_player.position.x, (int)_player.position.y)) _player.position.x += PLAYERSPEED;
		}
		if (_player.player_dir_right) {
			_player.position.x += PLAYERSPEED;
			if (!Game1.IsPlayerInMap((int)_player.position.x, (int)_player.position.y)) _player.position.x -= PLAYERSPEED;
		}
		//update player tick
		if (game_tick % 3 == 1) {
			player_tick++;
		}
	}
	playerDest.x = _player.position.x;
	playerDest.y = _player.position.y;

	//update tick counter
	game_tick++;

	// reset player tick
	if (player_tick > 12) {
		player_tick = 0;
	}

	//player animation
	switch (_player.dir) {
	case 1:
		player_texture = player_sprite_up;
		if (_player.isPlayerAttacking) {
			player_texture = player_sprite_slash[_player.dir];
			sword_texture = sword_sprite_slash[_player.dir];
			swordOffX = 12;
			swordOffY = 10;

		}
		playerSrc.x = playerSrc.width * player_tick;
		playerSrc.y = 0;
		break;
	case 0:
		player_texture = player_sprite_left;
		if (_player.isPlayerAttacking) {
			player_texture = player_sprite_slash[_player.dir];
			sword_texture = sword_sprite_slash[_player.dir];
			swordOffX = 25;
			swordOffY = -12;
		}
		playerSrc.x = playerSrc.width * player_tick;
		playerSrc.y = 0;
		break;
	case 2:
		player_texture = player_sprite_down;
		if (_player.isPlayerAttacking) {
			player_texture = player_sprite_slash[_player.dir];
			sword_texture = sword_sprite_slash[_player.dir];
			swordOffX = 12;
			swordOffY = -36;
		}
		playerSrc.x = playerSrc.width * player_tick;
		playerSrc.y = 0;
		break;
	case 3:
		player_texture = player_sprite_right;
		if (_player.isPlayerAttacking) {
			player_texture = player_sprite_slash[_player.dir];
			sword_texture = sword_sprite_slash[_player.dir];
			swordOffX = -25;
			swordOffY = -12;
		}
		playerSrc.x = playerSrc.width * player_tick;
		playerSrc.y = 0;
		break;
	}

	//set to default animation if player not moving
	if (!_player.isPlayerMoving) {
		if (_player.isPlayerAttacking) {
			player_texture = player_sprite_slash[2];
			sword_texture = sword_sprite_slash[2];
			swordOffX = 12;
			swordOffY = -36;
			atRest = true;
			playerSrc.x = playerSrc.width * player_tick;
			playerSrc.y = 0;
		}
		else {
			player_texture = player_sprite_down;;
			playerSrc.x = 0;
			playerSrc.y = 0;
		}
	}

	//camera
	monitor = GetCurrentMonitor();
	camera_offset = { float(GetMonitorWidth(monitor) / 2), float(GetMonitorHeight(monitor) / 2) };
	camera_pos = { float(playerDest.x - (playerDest.width / 2)),float(playerDest.y - (playerDest.height / 2)) };
	camera.target = { ((playerDest.x + (playerDest.width / 2)) - GetScreenWidth() / 2), float(playerDest.y + (playerDest.height / 2)) - GetScreenHeight() / 2 };

	//movement set to false
	_player.isPlayerMoving = false;
	_player.player_dir_up = false;
	_player.player_dir_down = false;
	_player.player_dir_left = false;
	_player.player_dir_right = false;

	//Dungeon
	Game1.Update(_player.position.x, _player.position.y, _player, pointer, pointerPlayer);
}

void marketupdate() {
	if (_player.isPlayerMoving) {
		if (_player.player_dir_up) {
			_player.position.y -= PLAYERSPEED;
		}
		if (_player.player_dir_down) {
			_player.position.y += PLAYERSPEED;
		}
		if (_player.player_dir_left) {
			_player.position.x -= PLAYERSPEED;
		}
		if (_player.player_dir_right) {
			_player.position.x += PLAYERSPEED;
		}
		//update player tick
		if (game_tick % 3 == 1) {
			player_tick++;
		}
	}
	playerDest.x = _player.position.x;
	playerDest.y = _player.position.y;

	//update tick counter
	game_tick++;

	// reset player tick
	if (player_tick > 12) {
		player_tick = 0;
	}

	//give player animation
	switch (_player.dir) {
	case 1:
		player_texture = player_sprite_up;
		playerSrc.x = playerSrc.width * player_tick;
		playerSrc.y = 0;
		break;
	case 0:
		player_texture = player_sprite_left;
		playerSrc.x = playerSrc.width * player_tick;
		playerSrc.y = 0;
		break;
	case 2:
		player_texture = player_sprite_down;
		playerSrc.x = playerSrc.width * player_tick;
		playerSrc.y = 0;
		break;
	case 3:
		player_texture = player_sprite_right;
		playerSrc.x = playerSrc.width * player_tick;
		playerSrc.y = 0;
		break;
	}

	//set to default animation if player not moving
	if (!_player.isPlayerMoving) {
		player_texture = player_sprite_down;
		playerSrc.x = 0;
		playerSrc.y = 0;
	}

	//camera
	monitor = GetCurrentMonitor();
	camera_offset = { float(GetMonitorWidth(monitor) / 2), float(GetMonitorHeight(monitor) / 2) };
	camera_pos = { float(playerDest.x - (playerDest.width / 2)),float(playerDest.y - (playerDest.height / 2)) };
	camera.target = { ((playerDest.x + (playerDest.width / 2)) - GetScreenWidth() / 2), float(playerDest.y + (playerDest.height / 2)) - GetScreenHeight() / 2 };

	//movement set to false
	_player.isPlayerMoving = false;
	_player.player_dir_up = false;
	_player.player_dir_down = false;
	_player.player_dir_left = false;
	_player.player_dir_right = false;

}



void render() {
	BeginDrawing();
	ClearBackground(WHITE);
	BeginMode2D(camera);

	//Draw dungeon
	
	Game1.Draw(Tile, _player);
	if (_player.isPlayerAttacking) {
		if (!(_player.dir == 2))DrawTexturePro(sword_texture, playerSrc, playerDest, { playerCenter.x + swordOffX,playerCenter.y + swordOffY }, 0, WHITE);
	}
	DrawTexturePro(player_texture, playerSrc, playerDest, playerCenter, 0, WHITE);
	if (_player.isPlayerAttacking) {
		if (_player.dir == 2 || atRest)DrawTexturePro(sword_texture, playerSrc, playerDest, { playerCenter.x + swordOffX,playerCenter.y + swordOffY }, 0, WHITE);
	}
	drawInventoryUI();
	EndMode2D();
	EndDrawing();
}

void quit() {
	CloseWindow();
}

void marketrender() {
	BeginDrawing();
	ClearBackground(BLACK);
	BeginMode2D(camera);

	trading_game();
	DrawTexturePro(player_texture, playerSrc, playerDest, playerCenter, 0, WHITE);
	drawInventoryUI();


	EndMode2D();
	EndDrawing();
}


int main() {
	initialise();

	while (!titlescreenshown&& !WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		DrawRectangle(0, 0, 10, 10, GREEN);
		
		DrawText("THE SPIRIT TRADER'S ODDESEY", 0, 0, 50, DARKGREEN);
		DrawText("PRESS ENTER TO PLAY", 0, 100, 25, DARKGREEN);
		DrawText("Controls :", 0, 150, 25, DARKGREEN);
		DrawText("1) WASD to MOVE ", 0, 200, 25, DARKGREEN);
		DrawText("2) SPACE to ATTACK", 0, 250, 25, DARKGREEN);
		DrawText("3) RIGHT CLICK to SHOW ITEMS", 0, 300, 25, DARKGREEN);
		DrawText("4) O (Letter) to teleport between Dungeon and Market", 0, 350, 25, DARKGREEN);


		EndDrawing();
		if (IsKeyPressed(KEY_ENTER)) {
			titlescreenshown = true;
		}
	}


	while (!WindowShouldClose()) {
		dungeoninitialiseloadingtiles();
		while (!WindowShouldClose() && !market_scene) {
			input();
			if (~_player.health <= 0) {
				update();
				render();
			}
			else
			{
				std::cout << "You Died" << std::endl;
			}
		}
		while (!WindowShouldClose() && market_scene) {
			marketinput();
			marketupdate();
			marketrender();
		}
		
	}
	quit();
	return 0;
}

/*
if (!titlescreenshown) {
	DrawRectangle(0, 0, 10, 10, GREEN);
	DrawText("The Spirit Trader's Odyssey", 20, 20, 40, DARKGREEN);
	DrawText("PRESS ENTER TO PLAY", 120, 220, 20, DARKGREEN);
	if (IsKeyPressed(KEY_ENTER)) {
		titlescreenshown = true;
	}
}*/

