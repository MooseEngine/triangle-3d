#include "linalg.h"

#ifndef MODEL_H
#define MODEL_H

static const int NUM_TRIANGLE_VERTICES = 3;
typedef struct {
	Vec3 vertices[3];
} Triangle;


/**
 * @brief Uses an edge function to determine whether a given vertex is within a triange.
 *
 * @param c The vertex to test.
 * @param t The triangle in question.
 *
 * @return Whether the vertex is in the triangle. 0: no, 1: yes.
 */
int isInsideTriangle(Vec3 c, Triangle t);

#endif /* MODEL_H */
