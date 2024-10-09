#include <Novice.h>
#include <Vector2.h>
#include "BasicFunctions.h"
#include <Block.h>

const char kWindowTitle[] = "GC1B_02_イグチ_GC1B_09_チョウイセン_GC1A_07_テュウタ_スカイビルダー";

float rightBorder = 528.0f;

Vector2 playerPos = { 200.0f, 100.0f };
Vector2 playerScreenPos = { 200.0f, 100.0f };


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 720, 720);

	int mockupGraph = Novice::LoadTexture("./Resources/Sprites/mockup1.png");
	int blockGraph = Novice::LoadTexture("./Resources/Sprites/basicBlock.png");

	

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

#pragma endregion

#pragma region convert2Screen

		playerScreenPos = World2Screen(playerPos, worldOrigin);
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
		
		
		Novice::DrawEllipse(static_cast<int>(playerScreenPos.x), static_cast<int>(playerScreenPos.y), 10, 10, 0.0f, BLUE, kFillModeSolid);

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
