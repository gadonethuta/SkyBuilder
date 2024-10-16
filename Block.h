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

float windSpeed = 0.0f;

float quakeValue = 0.0f;
float quakeLow = 0.0f;
float quakeHigh = 300.0f;

float highestBlockY = -1.0f;

const int kBlockNum = 12;  //配列の数
Block block[kBlockNum];

enum BlockState
{
	FLY,    //空中のブロック
	DROP,   //撃墜されたブロック
	IDLE,   //定点
};

void BlockCreate(Block &b, Vector2 pos, Vector2 speed, int colorRand){
		b.pos = pos;
		b.screenPos = { 0.0f, 0.0f };
		b.speed = speed;
		//here
		switch (colorRand) {
		case 0:
			b.wallColor = 0xFFFFFFFF;
			break;
		case 1://red
			b.wallColor = 0xD43867FF;
			break;
		case 2://blue
			b.wallColor = 0x2438BEFF;
			break;
		case 3://yellow
			b.wallColor = 0xFFBD39FF;
			break;
		case 4://GREEN
			//b.wallColor = 0x42C7BBFF;
			b.wallColor = 0x00C282FF;
			break;
		}
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
				//collision check
				for (int j = 0; j < kBlockNum; j++) {
					if (i != j && block[i].state == DROP && block[j].state == IDLE ) {
						/*if (CollisionVector2(block[i].pos, block[j].pos, 30.0f, 30.0f)) {
							block[i].speed.y = 0.0f;
							block[i].state = IDLE;
						}*/
						if (CollisionRect(block[i].pos, block[j].pos, block[i].width, block[i].height) && block[j].isTop) {
							block[i].speed.y = 0.0f;
							block[i].state = IDLE;
							block[i].pos.y = block[j].pos.y + block[i].height - 4.0f;
						} else if (CollisionRect(block[i].pos, block[j].pos, block[i].width, block[i].height)) {
							//block[i].speed.y = -5.0f;
						}
					}
				}

				block[i].pos.y -= block[i].speed.y;
				block[i].pos.x += windSpeed;

				if (block[i].pos.y < 0.0f) {
					block[i].pos.y = 0.0f;
					block[i].state = IDLE;
				}
				
				break;

			case IDLE:
				if (block[i].pos.y > highestBlockY) {
					block[i].isTop = true;
					highestBlockY = block[i].pos.y;
					for (int j = 0; j < kBlockNum; j++) {
						if (i != j) {
							block[j].isTop = false;
							if (block[j].pos.y + (block[j].height * 4) < block[i].pos.y) {
								block[j].isVisible = false;
							}
						}
					}
				}
				break;
			}
		}
	
	}
};