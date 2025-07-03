#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>
#include <cassert>
#include "types.hpp"

using namespace std;

const int scr_w = 700;
const int scr_h = 900;

void init_field() {
	for (int y = 0; y < f_h; y++) {
		for (int x = 0; x < f_w; x++) {
			field[y][x].type = EMPTY;
			field[y][x].color = RAYWHITE;
			field[y][x].is_updated = false;
		}
	}
	for (int j = 8; j < 50; j++) {
		if (j % 2 == 0) {
			continue;
		}
		field[f_h - 1][j].type = SAND;
	}
}

Vector2 convert_to_px(Vector2 v) {
	Vector2 v_px = { v.x * cell_size, v.y * cell_size };
	//centerize
	float w = scr_w/2 - f_w/2 * cell_size;
	float h = scr_h/2 - f_h/2 * cell_size;
	return { v_px.x + w , v_px.y + h };
}

void render_field() {
	Vector2 v2c_size = { cell_size, cell_size };
	for (int y = 0; y < f_h; y++) {
		for (int x = 0; x < f_w; x++) {
			Vector2 temp_pos = { float(x), float(y) };
			if (field[y][x].type == EMPTY) {
				DrawRectangleV(convert_to_px(temp_pos), v2c_size, field[y][x].color);
			}
			if (field[y][x].type == SAND) {
				DrawRectangleV(convert_to_px(temp_pos), v2c_size, BEIGE);
			}
		}
	}
}

void render() {
	BeginDrawing();
	ClearBackground(WHITE);
	render_field();
	EndDrawing();
}

int main() {
	InitWindow(scr_w, scr_h, "Falling sand");

	init_field();

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		render();
	}
	return 0;
}
