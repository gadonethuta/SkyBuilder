#include <Novice.h>
#include <Vector2.h>
#include "BasicFunctions.h"
#include "Block.h"
#include "Player.h"

const char kWindowTitle[] = "GC1B_02_イグチ_GC1B_09_チョウイセン_GC1A_07_テュウタ_スカイビルダー";

float rightBorder = 528.0f;

//Vector2 playerPos = { 200.0f, 100.0f };
//Vector2 playerScreenPos = { 200.0f, 100.0f };


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 720, 720);

	int mockupGraph = Novice::LoadTexture("./Resources/Sprites/mockup1.png");
	int blockGraph = Novice::LoadTexture("./Resources/Sprites/basicBlock.png");
	int pointerGraph = Novice::LoadTexture("./Resources/Sprites/pointer.png");
	int jumpLevelGraph = Novice::LoadTexture("./Resources/Sprites/jumpLevel.png");
	

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

#pragma region input

		if (keys[DIK_UP]) {
			worldOrigin.y -= 5.0f;
		} else if(keys[DIK_DOWN]) {
			worldOrigin.y += 5.0f;
		}
		
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			for (int i = 0; i < kBlockNum; i++) {
				block[i].state = DROP;
			}
		}

		if (keys[DIK_C] && !preKeys[DIK_C]) {
			PlayerInitialization(player, { 250,0 });
			for (int i = 0; i < kBlockNum; i++) {
				if (!block[i].isVisible) {
					BlockCreate(block[i], { 0,700 }, { 5, 15 });
					break;
				}
			}
		}

		

#pragma endregion

#pragma region update
		BlockUpdate();
		PlayerUpdate(player);
		
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			Jump(player, 35.0f);
		}

#pragma endregion

#pragma region convert2Screen

		player.screenPos = World2Screen(player.pos, worldOrigin);
		for (int i = 0; i < kBlockNum; i++) {
			block[i].screenPos = World2Screen(block[i].pos, worldOrigin);
		}

#pragma endregion

#pragma region draw

		Novice::DrawSprite(0 , 0 - int(worldOrigin.y), mockupGraph, 1.0f, 1.0f, 0.0f, 0x444444FF);
		
		for (int i = 0; i < kBlockNum; i++) {
			Novice::DrawSprite(int(block[i].screenPos.x- block[i].width/2), int(block[i].screenPos.y - block[i].height / 2), blockGraph, 1.0f, 1.0f, 0.0f, WHITE);
		}

		Novice::DrawEllipse(static_cast<int>(worldOrigin.x), static_cast<int>(worldOrigin.y), 10, 10, 0.0f, RED, kFillModeSolid);
		
		
		Novice::DrawEllipse(static_cast<int>(player.screenPos.x), static_cast<int>(player.screenPos.y), 100, 100, 0.0f, BLACK, kFillModeSolid);

		Novice::ScreenPrintf(0, 0, "PlayerPos : %.2f, %.2f", player.screenPos.x, player.screenPos.y);
		Novice::ScreenPrintf(0, 20, "jumpPower: %.2f", jumpPower);
		Novice::ScreenPrintf(0, 40, "gauge: %d", gauge);

#pragma endregion


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
