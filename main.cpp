#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

const int scr_w = 700;
const int scr_h = 900;

const int map_w = 30;
const int map_h = 30;

const int cell_size = 20;
const int grid_thickness = 1;


void render() {
	BeginDrawing();
	ClearBackground(BLACK);
	EndDrawing();
}

int main() {

	InitWindow(scr_w, scr_h, "Falling sand");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		render();
	}
	return 0;
}
