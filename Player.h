#pragma once
#pragma once
#include <Vector2.h>
#include "BasicFunctions.h"

struct Player {
	Vector2 pos;
	Vector2 screenPos;
	Vector2 velocity;
	int width;
	int height;
	bool isOnBlock;
};
Player player;

float acceleration = -0.8f;

int gauge = 0; //0~99
int gaugeSpeed = 2;
//int gaugeY = 0;

float jumpPower;
float jumpLevel[10] = { 0.5f, 0.7f, 0.8f, 0.8f, 0.85f, 0.85f, 0.7f, 0.7f, 1.0f, 0.7f };
//float jumpLevel[10] = { 0.3f, 0.5f, 0.6f, 0.6f, 0.8f, 0.8f, 0.5f, 0.5f, 1.0f, 0.5f };


void PlayerInitialization(Player& P, Vector2 pos/*, float speed*/) {
	P.pos = pos;
	P.screenPos = { 0.0f, 0.0f };
	P.velocity = { 0.0f, 0.0f };
	P.width = 64;
	P.height = 64;
	P.isOnBlock = true;
}

auto Jump(Player& P, float Jpower) { //jumpPower
	if (P.isOnBlock) {
		P.velocity.y = Jpower * jumpPower;
	} else {
		P.velocity.y = Jpower * 0.35f;
	}
	

}

void PlayerUpdate(Player& P) {

	gauge += gaugeSpeed;

	if (gauge <= 0 || gauge >= 99) {
		gaugeSpeed = -gaugeSpeed;
	}

	P.pos.y += P.velocity.y;
	P.velocity.y += acceleration;

	if (P.pos.y <= 0) {
		P.pos.y = 0;
	}

	jumpPower = jumpLevel[gauge / 10];

}