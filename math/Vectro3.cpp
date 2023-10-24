#include "Vector3.h"

float Distance(Vector3 vec1, Vector3 vec2) {
	float distance = sqrtf(powf(vec1.x - vec2.x, 2) + powf(vec1.y - vec2.y, 2) + powf(vec1.z - vec2.z, 2));
	return distance;
}

float Magnitude(Vector3 vec) {
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}