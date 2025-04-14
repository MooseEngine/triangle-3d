#include "model.h"
#include <math.h>

int isInsideTriangle(Vec3 c, Triangle t) {
	float signAB = signbit(edgeFunction(c, t.vertices[0], t.vertices[1]));
	float signBC = signbit(edgeFunction(c, t.vertices[1], t.vertices[2]));
	float signCA = signbit(edgeFunction(c, t.vertices[2], t.vertices[0]));

	return (signAB == signBC) && (signBC == signCA);
}
