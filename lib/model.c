#include "model.h"
#include <math.h>

int isInsideTriangle(Vec3 p, Triangle t) {
	Vec3 baryCoords = computeBarycentricCoordinates(p, t);
	return baryCoords.x >= 0 && baryCoords.y >= 0 && baryCoords.z >= 0;
}

Vec3 computeBarycentricCoordinates(Vec3 p, Triangle t) {
	float denom = ((t.vertices[1].y - t.vertices[2].y) * (t.vertices[0].x - t.vertices[2].x) + (t.vertices[2].x - t.vertices[1].x) * (t.vertices[0].y - t.vertices[2].y));

	float alpha = ((t.vertices[1].y - t.vertices[2].y) * (p.x - t.vertices[2].x) + (t.vertices[2].x - t.vertices[1].x) * (p.y - t.vertices[2].y)) / denom;
	float beta  = ((t.vertices[2].y - t.vertices[0].y) * (p.x - t.vertices[2].x) + (t.vertices[0].x - t.vertices[2].x) * (p.y - t.vertices[2].y)) / denom;
	float gamma = 1.0f - alpha - beta;

	return (Vec3) { alpha, beta, gamma };
}
