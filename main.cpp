#include <Novice.h>
#include <Vector2.h>
#include <stdio.h>
#include <time.h>

#include "Value.h"
#include "Player.h"
#include "BasicFunctions.h"
#include "Block.h"
#include "Particle.h"


const char kWindowTitle[] = "GC1B_02_イグチ_GC1B_09_チョウイセン_GC1A_07_テュウタ_スカイビルダー";

float rightBorder = 528.0f;
ValueInt blockTimer = { 0, 150 };

enum GameState {
	TITLE,
	GAME,
	OVER
};

GameState currentGameState = TITLE;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 720, 720);

	srand(static_cast<int>(time(nullptr)));

	int skyGraph = Novice::LoadTexture("./Resources/Sprites/sky.png");
	int bigCloudGraph = Novice::LoadTexture("./Resources/Sprites/bigCloud.png");
	int farCityGraph = Novice::LoadTexture("./Resources/Sprites/farCity.png");
	int nearCityGraph = Novice::LoadTexture("./Resources/Sprites/nearCity.png");
	int gradientGraph = Novice::LoadTexture("./Resources/Sprites/gradient.png");
	int uiBaseGraph = Novice::LoadTexture("./Resources/Sprites/UIBase.png");
	int roadGraph = Novice::LoadTexture("./Resources/Sprites/road.png");
	float cloudParallaxScroll = 0;
	float cloudParallaxScrollSpeed = -0.2f;

	int blockGraph = Novice::LoadTexture("./Resources/Sprites/basicBlock.png");
	int pointerGraph = Novice::LoadTexture("./Resources/Sprites/pointer.png");
	int jumpLevelGraph = Novice::LoadTexture("./Resources/Sprites/jumpLevel.png");

	PlayerInitialization(player, { 250,0 });
	worldOrigin.y = 720.0f + highestBlockY - 190.0f;

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		switch (currentGameState) {
		case TITLE: {

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				currentGameState = GAME;
			}

			Novice::DrawBox(0, 0, 800, 800, 0.0f, BLUE, kFillModeSolid);
			break;
		}
		case GAME: {
#pragma region input

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				for (int i = 0; i < kBlockNum; i++) {
					if (block[i].state == FLY) {
						block[i].state = DROP;
					}
				}
			}

			if (keys[DIK_S] && !preKeys[DIK_S] && player.isOnBlock) {
				Jump(player, 35.0f);
			}
			if (keys[DIK_S] && !preKeys[DIK_S] && !player.isOnBlock) {
				for (int i = 0; i < kBlockNum; i++) {
					if (block[i].state == FLY) {
						if (player.pos.y >= block[i].pos.y) {
							player.pos.x = block[i].pos.x;
							player.pos.y = block[i].pos.y + block[i].height / 2.0f;
							Jump(player, 35.0f);
							block[i].state = DROP;
						}
					}
				}
			}

			//debug windSpeed
			if (keys[DIK_Q] && !preKeys[DIK_Q]) {
				windSpeed -= 2.0f;
			}
			if (keys[DIK_W] && !preKeys[DIK_W]) {
				windSpeed = 0.0f;
			}
			if (keys[DIK_E] && !preKeys[DIK_E]) {
				windSpeed += 2.0f;
			}

#pragma endregion

#pragma region update

			//block
			blockTimer.now++;
			if (blockTimer.now >= blockTimer.max) {
				blockTimer.now = 0;
				for (int i = 0; i < kBlockNum; i++) {
					if (!block[i].isVisible) {
						BlockCreate(block[i], { 500, worldOrigin.y - 200.0f }, { -5, 25 }, rand() % 5);
						break;
					}
				}
			}

			BlockUpdate();

			//player
			PlayerUpdate(player);

			for (int i = 0; i < kBlockNum; i++) {
				if (block[i].isTop) {
					player.pos.x = Lerp(player.pos.x, block[i].pos.x, 0.05f);

					if (player.pos.y <= block[i].pos.y + block[i].height / 2.0f) {
						player.pos.y = block[i].pos.y + block[i].height / 2.0f;
						player.velocity.y = 0.0f;
						player.isOnBlock = true;
					} else {
						player.isOnBlock = false;
					}
				}
			}

			//particle
			Novice::ScreenPrintf(0, 690, "World Y : %.2f", worldOrigin.y);
			if (windSpeed != 0) {
				particleTimer.now++;
				if (particleTimer.now > particleTimer.max) {
					particleTimer.now = 0;
					//create particle here
					for (int i = 0; i < kParticleCount; i++) {
						if (!windParticle[i].isVisible) {
							if (windSpeed > 0.0f) {
								ParticleCreate(windParticle[i], { -20.0f, (worldOrigin.y - rand() % 700) });
								break;
							} else {
								ParticleCreate(windParticle[i], { 550.0f, (worldOrigin.y - rand() % 700) });
								break;
							}

						}
					}
				}
			}
			ParticleUpdate(windSpeed);

			//BG
			cloudParallaxScroll += cloudParallaxScrollSpeed;
			if (cloudParallaxScroll <= -800.0f) {
				cloudParallaxScroll = 0.0f;
			}
			//カメラ
			worldOrigin.y = Lerp(worldOrigin.y, 720.0f + highestBlockY - 190.0f, 0.05f);

#pragma endregion

#pragma region convert2Screen

			player.screenPos = World2Screen(player.pos, worldOrigin);
			for (int i = 0; i < kBlockNum; i++) {
				block[i].screenPos = World2Screen(block[i].pos, worldOrigin);
			}
			for (int i = 0; i < kParticleCount; i++) {
				if (windParticle[i].isVisible) {
					windParticle[i].screenPos = World2Screen(windParticle[i].pos, worldOrigin);
				}
			}

#pragma endregion

#pragma region draw

			//BG
			Novice::DrawBox(0, 0, 720, 720, 0.0f, 0x411F93FF, kFillModeSolid);

			Novice::DrawSprite(0, -3252 + int(worldOrigin.y), skyGraph, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(int(cloudParallaxScroll), 0 + int(worldOrigin.y * 0.25f), bigCloudGraph, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(int(cloudParallaxScroll) + 800, 0 + int(worldOrigin.y * 0.25f), bigCloudGraph, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(140, -350 + int(worldOrigin.y * 0.4f), farCityGraph, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(140, -150 + int(worldOrigin.y * 0.6f), nearCityGraph, 1.0f, 1.0f, 0.0f, WHITE);

			Novice::SetBlendMode(kBlendModeScreen);
			Novice::DrawSprite(70, 0, gradientGraph, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::SetBlendMode(kBlendModeNormal);

			Novice::DrawSprite(140, int(worldOrigin.y) + 28, roadGraph, 1.0f, 1.0f, 0.0f, WHITE);


			//Novice::DrawSprite(0 , 0/* - int(worldOrigin.y)*/, mockupGraph, 1.0f, 1.0f, 0.0f, 0xFFFFFF22);

			for (int i = 0; i < kBlockNum; i++) {
				if (block[i].isVisible) {
					Novice::DrawBox(int(block[i].screenPos.x + 4 - block[i].width / 2), int(block[i].screenPos.y - block[i].height / 2), 92, 60, 0.0f, 0x86DDFFFF, kFillModeSolid);
					Novice::DrawSprite(int(block[i].screenPos.x - block[i].width / 2), int(block[i].screenPos.y - block[i].height / 2), blockGraph, 1.0f, 1.0f, 0.0f, block[i].wallColor);
					//Novice::DrawEllipse(int(block[i].screenPos.x), int(block[i].screenPos.y), 30, 30, 0.0f, RED, kFillModeWireFrame);
					if (block[i].state == DROP) {
						Novice::DrawBox(int(block[i].screenPos.x + 4 - block[i].width / 2), int(block[i].screenPos.y - block[i].height / 2), 92, 60, 0.0f, BLUE, kFillModeWireFrame);
					} else {
						Novice::DrawBox(int(block[i].screenPos.x + 4 - block[i].width / 2), int(block[i].screenPos.y - block[i].height / 2 + 4.0f), 92, 60, 0.0f, RED, kFillModeWireFrame);
					}
				}
			}

			Novice::DrawEllipse(static_cast<int>(worldOrigin.x), static_cast<int>(worldOrigin.y), 10, 10, 0.0f, RED, kFillModeSolid);


			Novice::DrawEllipse(static_cast<int>(player.screenPos.x), static_cast<int>(player.screenPos.y), 10, 10, 0.0f, BLACK, kFillModeSolid);


			//particle
			for (int i = 0; i < kParticleCount; i++) {
				if (windParticle[i].isVisible) {
					Novice::DrawEllipse(static_cast<int>(windParticle[i].screenPos.x), static_cast<int>(windParticle[i].screenPos.y), 4, 4, 0.0f, 0xFFFFFFAA, kFillModeSolid);
				}
			}


			Novice::ScreenPrintf(0, 0, "Highest Block Y : %.2f", highestBlockY);
			Novice::ScreenPrintf(0, 20, "IsOnBlock : %d", player.isOnBlock);
			Novice::ScreenPrintf(230, 0, "CurrentHeight : %.2f", highestBlockY / 60.0f);
			Novice::ScreenPrintf(230, 20, "WindSpeed : %.2f", windSpeed);


			for (int i = 0; i < kBlockNum; i++) {
				Novice::ScreenPrintf(0, 50 + (i * 20), "isTop[%d] : %d", i, block[i].isTop);
			}


			//UI
			Novice::DrawSprite(0, 0, uiBaseGraph, 1.0f, 1.0f, 0.0f, WHITE);

			Novice::DrawSprite(100, 400, jumpLevelGraph, 1.0f, 1.0f, 0.0f, WHITE);

			Novice::DrawSprite(28, 400 + int(240 - gauge * 2.4f - 32), pointerGraph, 1.0f, 1.0f, 0.0f, WHITE);


#pragma endregion

			break;
		}
		case OVER: {
			break;
		}
		}




		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
