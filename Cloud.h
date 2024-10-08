#pragma once
#include <Vector2.h>
#include <math.h>

struct Cloud {
	Vector2 pos;
	Vector2 screenPos;
	float scaleY;
	float offsetY;
	unsigned int lowerColor;
};

extern Cloud cloud[5];

extern float cloudScrollSpeed = 1.5f;

void CreateCloud() {
	
}

void CloudUpdate(Cloud& c, float cloudSpeed) {
	c.scaleY = fabsf(c.pos.y - 360.0f) / 240.0f;
	if (c.scaleY <= 0.11f) {
		c.scaleY = 0.0f;
	}
	if (c.pos.y <= 360.0f) {
		c.lowerColor = 0xB9B7Dfff;

	} else {
		c.lowerColor = 0xFFFFFF;
	}

	c.pos.x += cloudSpeed;
	if (c.pos.x > 720.0f + 88.0f) {
		c.pos.x = -88.0f;
	}
}