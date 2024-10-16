#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <Vector2.h>
#include "Block.h"
#include "Value.h"

struct Particle {
	Vector2 pos;
	Vector2 screenPos;
	Vector2 velocity;
	float theta;
	bool isVisible;
};

Value particleTimer = { 0, 5 };

const int kParticleCount = 50;
Particle windParticle[kParticleCount];

void ParticleCreate(Particle& Parti, Vector2 pos) {
	Parti.pos = pos;
	Parti.screenPos = { 0.0f, 0.0f };
	Parti.velocity = { 0.0f, 0.0f };
	Parti.isVisible = true;
	Parti.theta = 0;
}

void ParticleUpdate(float wSpeed) {
	
	//float pi = float(M_PI);
	//float amplitude = 100.0f;
	for (int i = 0; i < kParticleCount; i++) {
		if (windParticle[i].isVisible) {
			windParticle[i].theta += 0.075f;

			windParticle[i].velocity.x = 2 * wSpeed;

			windParticle[i].pos.x += windParticle[i].velocity.x;
			//windParticle[i].pos.y += windParticle[i].velocity.y;
			windParticle[i].pos.y += sinf(windParticle[i].theta);
			
			if (windParticle[i].pos.x > 600.0f || windParticle[i].pos.x < -50.0f) {
				windParticle[i].isVisible = false;
			}
		}
	}
	
}