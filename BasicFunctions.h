#pragma once
#include <Vector2.h>

Vector2 worldOrigin = { 192.0f, 720.0f };

/// <summary>
/// ワールド座標からスクリーン座標に変更
/// </summary>
/// <param name="position">変わりたい座標</param>
/// <param name="world_origin">ワールド座標の中心</param>
/// <returns>スクリーン座標0</returns>
Vector2 World2Screen(Vector2 position, Vector2 world_origin);