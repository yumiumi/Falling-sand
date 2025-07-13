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
	//field[0][0].type = SAND;
	//field[0][9].type = SAND;
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

void can_place(int x, int y, Cell* block) {
	// [0] (x, y)
	if (y < 0 || y >= f_h || x < 0 || x >= f_w) {
		block[0].type = BORDER;
	}
	else {
		block[0].type = field[y][x].type;
	}

	// [1] (y + 1, x)
	if (y + 1 < 0 || y + 1 >= f_h || x < 0 || x >= f_w) {
		block[1].type = BORDER;
	}
	else {
		block[1].type = field[y + 1][x].type;
	}

	// [2] (y, x + 1) 
	if (y < 0 || y >= f_h || x + 1 < 0 || x + 1 >= f_w) {
		block[2].type = BORDER;
	}
	else {
		block[2].type = field[y][x + 1].type;
	}

	// [3] (y + 1, x + 1)
	if (y + 1 < 0 || y + 1 >= f_h || x + 1 < 0 || x + 1 >= f_w) {
		block[3].type = BORDER;
	}
	else {
		block[3].type = field[y + 1][x + 1].type;
	}
}

void bc_automaton() {
	Cell block[4];
	//1
	for (int y = 0; y < f_h; y += 2) {
		for (int x = 0; x < f_w; x += 2) {
			can_place(x, y, block);
			block_physics(block);
			if (y >= 0 && y < f_h && x >= 0 && x < f_w) {
				field[y][x].type = block[0].type;
			}
			if (y + 1 >= 0 && y + 1 < f_h && x >= 0 && x < f_w) {
				field[y + 1][x].type = block[1].type;
			}
			if (y >= 0 && y < f_h && x + 1 >= 0 && x + 1 < f_w) {
				field[y][x + 1].type = block[2].type;
			}
			if (y + 1 >= 0 && y + 1 < f_h && x + 1 >= 0 && x + 1 < f_w) {
				field[y + 1][x + 1].type = block[3].type;
			}
		}
	}
	//2
	for (int y = 0; y < f_h + 1; y += 2) {
		for (int x = 0; x < f_w + 1; x += 2) {
			int y2 = y;
			int x2 = x + 1;
			can_place(x2, y2, block);
			block_physics(block);
			if (y2 >= 0 && y2 < f_h && x2 >= 0 && x2 < f_w) {
				field[y2][x2].type = block[0].type;
			}
			if (y2 + 1 >= 0 && y2 + 1 < f_h && x2 >= 0 && x2 < f_w) {
				field[y2 + 1][x2].type = block[1].type;
			}
			if (y2 >= 0 && y2 < f_h && x2 + 1 >= 0 && x2 + 1 < f_w) {
				field[y2][x2 + 1].type = block[2].type;
			}
			if (y2 + 1 >= 0 && y2 + 1 < f_h && x2 + 1 >= 0 && x2 + 1 < f_w) {
				field[y2 + 1][x2 + 1].type = block[3].type;
			}
		}
	}
	//3
	for (int y = 0; y < f_h + 1; y += 2) {
		for (int x = 0; x < f_w + 1; x += 2) {
			int y3 = y + 1;
			int x3 = x;
			can_place(x3, y3, block);
			block_physics(block);
			if (y3 >= 0 && y3 < f_h && x3 >= 0 && x3 < f_w) {
				field[y3][x3].type = block[0].type;
			}
			if (y3 + 1 >= 0 && y3 + 1 < f_h && x3 >= 0 && x3 < f_w) {
				field[y3 + 1][x3].type = block[1].type;
			}
			if (y3 >= 0 && y3 < f_h && x3 + 1 >= 0 && x3 + 1 < f_w) {
				field[y3][x3 + 1].type = block[2].type;
			}
			if (y3 + 1 >= 0 && y3 + 1 < f_h && x3 + 1 >= 0 && x3 + 1 < f_w) {
				field[y3 + 1][x3 + 1].type = block[3].type;
			}
		}
	}
	//4
	for (int y = 0; y < f_h + 1; y += 2) {
		for (int x = 0; x < f_w + 1; x += 2) {
			int y4 = y + 1;
			int x4 = x + 1;
			can_place(x4, y4, block);
			block_physics(block);
			if (y4 >= 0 && y4 < f_h && x4 >= 0 && x4 < f_w) {
				field[y4][x4].type = block[0].type;
			}
			if (y4 + 1 >= 0 && y4 + 1 < f_h && x4 >= 0 && x4 < f_w) {
				field[y4 + 1][x4].type = block[1].type;
			}
			if (y4 >= 0 && y4 < f_h && x4 + 1 >= 0 && x4 + 1 < f_w) {
				field[y4][x4 + 1].type = block[2].type;
			}
			if (y4 + 1 >= 0 && y4 + 1 < f_h && x4 + 1 >= 0 && x4 + 1 < f_w) {
				field[y4 + 1][x4 + 1].type = block[3].type;
			}
		}
	}
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
