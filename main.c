#include <complex.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <SDL2/SDL.h>

// 0xAARRGGBB
#define WHITE	0xFFFFFFFF;
#define BLACK	0xFF000000;
#define RED	0xFFFF0000;
#define ORANGE	0xFFFFA500;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct {
	float x;
	float y;
} Coordinate;

typedef struct {
	Coordinate A;
	Coordinate B;
	Coordinate C;
} Triangle;

/**
 * @brief Computes whether a given coordinate is on one side or the other of a line formed by two other Coordinates.
 *
 * @param c The coordinate in question.
 * @param A The first coordinate forming a line.
 * @param B The coordinate the line is formed with.
 *
 * @return The result of the edge function.
 */
float edgeFunction(Coordinate c, Coordinate A, Coordinate B) {
	return (c.y - A.y) * (B.x - A.x) - (c.x - A.x) * (B.y - A.y);
}

/**
 * @brief Uses an edge function to determine whether a given coordinate is within a triange.
 *
 * @param c The coordinate to test.
 * @param t The triangle in question.
 *
 * @return Whether the coordinate is in the triangle. 0: no, 1: yes.
 */
int isInsideTriangle(Coordinate c, Triangle t) {
	float signAB = signbit(edgeFunction(c, t.A, t.B));
	float signBC = signbit(edgeFunction(c, t.B, t.C));
	float signCA = signbit(edgeFunction(c, t.C, t.A));

	return (signAB == signBC) && (signBC == signCA);
}

int main() {
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

	const Triangle t = {
		.A = { .x = 502, .y = 3   },
		.B = { .x = 381, .y = 242 },
		.C = { .x = 119, .y = 113 }
	};

	// Fill framebuffer with white if not inside triangle.
	// If inside triangle, make it ORANGE
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		// You can optimize this by calculating the max and min x and y values that the triangle can be.
		Coordinate c = { .x = i % SCREEN_WIDTH, .y = i / SCREEN_WIDTH };
		if (isInsideTriangle(c, t)) {
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



