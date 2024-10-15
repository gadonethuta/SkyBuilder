#pragma once
#include <Vector2.h>
#include "BasicFunctions.h"

struct Block{
	Vector2 pos ;
	Vector2 screenPos;
	Vector2 speed ;
	unsigned int wallColor;
	bool isTop;        //最高なのか
	bool isVisible = false;    //見えるのか
	int state;
	int width;
	int height;
};

float windSpeed = 2.0f;

const int kBlockNum = 10;  //配列の数
Block block[kBlockNum];

enum BlockState
{
	FLY,    //空中のブロック
	DROP,   //撃墜されたブロック
	IDLE,   //定点
};

void BlockCreate(Block &b, Vector2 pos, Vector2 speed){
		b.pos = pos;
		b.screenPos = { 0.0f, 0.0f };
		b.speed = speed;
		b.wallColor = WHITE;
		b.isTop = false;
		b.isVisible = true;
		b.state = 0;
		b.width = 100;
		b.height = 64;
}


void BlockUpdate() {
	
	for (int i = 0; i < kBlockNum; i++) {

		if(block[i].isVisible){
			
			switch (block[i].state) {
			case FLY:
				block[i].pos.x += block[i].speed.x;

				if (block[i].pos.x > 700.0f || block[i].pos.x < -100.0f) {
					block[i].isVisible = false;
				}
				break;

			case DROP:
				block[i].pos.y -= block[i].speed.y;
				block[i].pos.x += windSpeed;

				if (block[i].pos.y < 0) {
					block[i].isVisible = false;
				}
				
				break;

			case IDLE:

				break;
			}
		}
	
	}
};