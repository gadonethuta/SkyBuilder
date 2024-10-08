#include <Vector2.h>

Vector2 World2Screen(Vector2 position, Vector2 world_origin) {
	position.x += world_origin.x;
	position.y = -(position.y - world_origin.y);
	return position;
}