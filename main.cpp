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
}

void block_physics(Cell* block) {
	if (block[0].type == SAND && block[1].type == EMPTY) {
		block[0].type = EMPTY;
		block[1].type = SAND;
	}
	if (block[2].type == SAND && block[3].type == EMPTY) {
		block[2].type = EMPTY;
		block[3].type = SAND;
	}
	if (block[0].type == SAND && block[1].type == SAND) {
		if (block[3].type == EMPTY) {
			block[0].type = EMPTY;
			block[3].type = SAND;
		}
	}
	if (block[2].type == SAND && block[3].type == SAND) {
		if (block[1].type == EMPTY) {
			block[2].type = EMPTY;
			block[1].type = SAND;
		}
	}
}

bool is_valid(int x, int y) {
	return y >= 0 && y < f_h && x >= 0 && x < f_w;
}

Cell_Type get_if_valid(int x, int y) {
	if (is_valid(x, y)) {
		return field[y][x].type;
	}
	else {
		return BORDER;
	}
}

void classify_block(int x, int y, Cell* block) {
	block[0].type = get_if_valid(x + 0, y + 0);
	block[1].type = get_if_valid(x + 0, y + 1);
	block[2].type = get_if_valid(x + 1, y + 0);
	block[3].type = get_if_valid(x + 1, y + 1);
}

void save_if_valid(Cell_Type type, int x, int y) {
	if (is_valid(x, y)) {
		field[y][x].type = type;
	}
}

void run_block_pass(int start_x, int start_y) {
	for (int y = start_y; y < f_h; y += 2) {
		for (int x = start_x; x < f_w; x += 2) {
			Cell block[4];
			classify_block(x, y, block);
			block_physics(block);

			save_if_valid(block[0].type, x + 0, y + 0);
			save_if_valid(block[1].type, x + 0, y + 1);
			save_if_valid(block[2].type, x + 1, y + 0);
			save_if_valid(block[3].type, x + 1, y + 1);
		}
	}
}

void bc_automaton() {
	run_block_pass(0, 0);
	run_block_pass(1, 0);
	run_block_pass(0, 1);
	run_block_pass(1, 1);
}

void draw() {
	float w = scr_w/2 - f_w/2 * cell_size;
	float h = scr_h/2 - f_h/2 * cell_size;
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		Vector2 mouse_px = GetMousePosition();
		mouse_px = { mouse_px.x - w, mouse_px.y - h };
		// Convert mouse position from pixels to cells
		Vector2 mouse_cell = { floor(mouse_px.x/cell_size), floor(mouse_px.y/cell_size) };
		
		if (int(mouse_cell.y) >= 0 && int(mouse_cell.y) < f_h && int(mouse_cell.x) >= 0 && int(mouse_cell.x) < f_w) {
			field[int(mouse_cell.y)][int(mouse_cell.x)].type = SAND;
		}
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
				DrawRectangleV(convert_to_px(temp_pos), v2c_size, RAYWHITE);
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
		draw();
		bc_automaton();
		render();
	}
	return 0;
}
