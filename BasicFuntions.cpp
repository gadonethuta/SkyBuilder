#include <Vector2.h>

Vector2 World2Screen(Vector2 position, Vector2 world_origin) {
	position.x += world_origin.x;
	position.y = -(position.y - world_origin.y);
	return position;
}

float Lerp(float from, float to, float rate) {
	float difference = to - from;
	from += difference * rate;
	return from;
}

float Clamp(float value, float min, float max) {
	if (value > max) {
		return max;
	} else if (value < min) {
		return min;
	} else {
		return value;
	}
}