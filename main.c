#include <complex.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <SDL2/SDL.h>

#include "lib/linalg.h"

// 0xAARRGGBB
#define WHITE	0xFFFFFFFF
#define BLACK	0xFF000000
#define RED	0xFFFF0000
#define ORANGE	0xFFFFA500

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

static const int NUM_TRIANGLE_VERTICES = 3;
typedef struct {
	Vec3 vertices[3];
} Triangle;

typedef struct {
	float minX, maxX, minY, maxY;
} BoundingBox;

// TODO: edgeFunction and isInsideTriangle now have to be changed to account for 3D. Use Barycentrics.
/**
 * @brief Computes whether a given vertex is on one side or the other of a line formed by two other Vertices.
 *
 * @param c The vertex in question.
 * @param A The first vertex forming a line.
 * @param B The vertex the line is formed with.
 *
 * @return The result of the edge function.
 */
float edgeFunction(Vec3 c, Vec3 A, Vec3 B) {
	return (c.y - A.y) * (B.x - A.x) - (c.x - A.x) * (B.y - A.y);
}

/**
 * @brief Uses an edge function to determine whether a given vertex is within a triange.
 *
 * @param c The vertex to test.
 * @param t The triangle in question.
 *
 * @return Whether the vertex is in the triangle. 0: no, 1: yes.
 */
int isInsideTriangle(Vec3 c, Triangle t) {
	float signAB = signbit(edgeFunction(c, t.vertices[0], t.vertices[1]));
	float signBC = signbit(edgeFunction(c, t.vertices[1], t.vertices[2]));
	float signCA = signbit(edgeFunction(c, t.vertices[2], t.vertices[0]));

	return (signAB == signBC) && (signBC == signCA);
}

BoundingBox calculateBoundingBox(Triangle transformedTriangle) {
	// TODO: implement
}

int main() {
	// Prepare transformation matrices
	const Triangle t = { .vertices = {
		{ .x = 502, .y = 3  , .z = 200 },
		{ .x = 381, .y = 242, .z = 300 },
		{ .x = 119, .y = 113, .z = 400 }
	}};
	Triangle tScreen = { .vertices = {
		{ .x = 0, .y = 0, .z = 0 },
		{ .x = 0, .y = 0, .z = 0 },
		{ .x = 0, .y = 0, .z = 0 }
	}};
	const Camera camera = {
		.position	= (Vec3) { 334, 119, 0	},
		.target		= (Vec3) { 334, 119, 300}, // center of triangle
		.up			= (Vec3) { 0  , 1  , 0	}
	};

	const float fov		= deg2rad(60.0f);
	const float aspect	= (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	const float near	= 1.0f;
	const float far		= 1000.0f;

	Mat4 modelMatrix = getModelMatrix();
	Mat4 viewMatrix = getViewMatrix(camera);
	Mat4 projMatrix = getProjectionMatrix(fov, aspect, near, far);

	// for each vertex in your triangle...
	for (int i = 0; i < NUM_TRIANGLE_VERTICES; i++) {
		Vec3 v = t.vertices[i];

		// Convert the vertex to homogenous coordinates
		Vec4 v4 = { v.x, v.y, v.z, 1.0f };

		// Apply transformations: model -> view -> projection

		Vec4 transformed = multiplyMat4Vec4(
				projMatrix, multiplyMat4Vec4(
				viewMatrix, multiplyMat4Vec4(
				modelMatrix, v4
			)));

		transformed.x /= transformed.w;
		transformed.y /= transformed.w;
		transformed.z /= transformed.w;

		tScreen.vertices[i].x = (int)((transformed.x + 1.0f) * 0.5f * SCREEN_WIDTH); // screenX
		tScreen.vertices[i].y = (int)((1.0f - (transformed.y + 1.0f) * 0.5f) * SCREEN_HEIGHT); // screenY
		tScreen.vertices[i].z = transformed.z; // depth for depth buffering
	}

	// Rasterize the triangle
	// BoundingBox boundingBox = calculateBoundingBox(tScreen);
	
	// printf("tScreen vertices:\n%f, %f, %f\n", tScreen.vertices[0].x, tScreen.vertices[0].y, tScreen.vertices[0].z);
	// printf("%f, %f, %f\n", tScreen.vertices[1].x, tScreen.vertices[1].y, tScreen.vertices[1].z);
	// printf("%f, %f, %f\n", tScreen.vertices[2].x, tScreen.vertices[2].y, tScreen.vertices[2].z);
	

	
	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	// Create a window
	SDL_Window *window = SDL_CreateWindow(
		"Software Rasterizer",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
	);
	if(window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		goto cleanup_window;
	}

	// Create a renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL) {
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		goto cleanup_renderer;
	}

	// Create a texture that will serve as our framebuffer.
	// Each pixel is 32 bits (ARGB format)
	SDL_Texture *texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT
	);
	if(texture == NULL) {
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		goto cleanup_texture;
	}

	uint32_t *framebuffer = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));
	if(framebuffer == NULL) {
		fprintf(stderr, "Failed to allocate memory for the framebuffer.\n");
		goto cleanup_framebuffer;
	}

	// Fill framebuffer with white if not inside triangle.
	// If inside triangle, make it ORANGE
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		// You can optimize this by calculating the max and min x and y values that the triangle can be.
		Vec3 c = { .x = i % SCREEN_WIDTH, .y = i / SCREEN_WIDTH };
		if (isInsideTriangle(c, tScreen)) {
			framebuffer[i] = ORANGE;
		} else {
			framebuffer[i] = BLACK;
		}
	}
	
	// begin render loop
	{
		int quit = 0;
		SDL_Event event;
		while (!quit) {
			// handle any events
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) quit = 1;
			}

			// update the texture with the framebuffer content
			// The last parameter is the `pitch` of the pixel data
			// `pitch`/`stride` is the number of bytes between the start of one row of pixel data
			// and the start of the next row (in memory)
			SDL_UpdateTexture(texture, NULL, framebuffer, SCREEN_WIDTH * sizeof(uint32_t));

			// render the texture
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);

			// cap the frame rate at ~60fps
			// 1000ms / 60 ~= 1 frame every 16.67 ms
			SDL_Delay(16);
		}
	}

cleanup_framebuffer:
	if (framebuffer) {
		free(framebuffer);
		framebuffer = NULL;
	}
cleanup_texture:
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
cleanup_renderer:
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
cleanup_window:
	if (window) {
		SDL_DestroyWindow(window);
		window = NULL;
	}
	SDL_Quit();

	return 0;
}



