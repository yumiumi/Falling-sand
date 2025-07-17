#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>
#include <cassert>
#include "types.hpp"
#include "imgui.h"
#include "rlImGui.h"

using namespace std;

const int scr_w = 1920;
const int scr_h = 1080;

void init_field() {
	for (int y = 0; y < f_h; y++) {
		for (int x = 0; x < f_w; x++) {
			field[y][x].type = EMPTY;
			field[y][x].color = BLACK;
			field[y][x].is_updated = false;
		}
	}
}

enum BlockQuadrant {
	TOP_LEFT = 0,
	BOT_LEFT = 1,
	TOP_RIGHT = 2,
	BOT_RIGHT = 3,
};

void block_physics(Cell* block) {
	if (material[block[TOP_LEFT].type].density > material[block[BOT_LEFT].type].density) {
		std::swap(block[TOP_LEFT], block[BOT_LEFT]);
	}
	if (material[block[TOP_RIGHT].type].density > material[block[BOT_RIGHT].type].density) {
		std::swap(block[TOP_RIGHT], block[BOT_RIGHT]);
	}
	if (material[block[TOP_LEFT].type].density <= material[block[BOT_LEFT].type].density && block[TOP_LEFT].type != BORDER) {
		if (material[block[BOT_RIGHT].type].density < material[block[TOP_LEFT].type].density) {
			std::swap(block[TOP_LEFT], block[BOT_RIGHT]);
		}
	}
	if (material[block[TOP_RIGHT].type].density <= material[block[BOT_RIGHT].type].density && block[TOP_RIGHT].type != BORDER) {
		if (material[block[BOT_LEFT].type].density < material[block[TOP_RIGHT].type].density) {
			std::swap(block[TOP_RIGHT], block[BOT_LEFT]);
		}
	}

	if (material[block[BOT_LEFT].type].density >= material[block[TOP_LEFT].type].density
		&& material[block[BOT_RIGHT].type].density >= material[block[TOP_LEFT].type].density
		&& material[block[TOP_LEFT].type].is_liquid == true) {
		if (material[block[TOP_RIGHT].type].density < material[block[TOP_LEFT].type].density) {
			int n = GetRandomValue(1, 100);
			if (n <= 70) {
				std::swap(block[TOP_LEFT], block[TOP_RIGHT]);
			}
		}
	}
	else if (material[block[BOT_RIGHT].type].density >= material[block[TOP_RIGHT].type].density
		&& material[block[BOT_LEFT].type].density >= material[block[TOP_RIGHT].type].density
		&& material[block[TOP_RIGHT].type].is_liquid == true) {
		if (material[block[TOP_LEFT].type].density < material[block[TOP_RIGHT].type].density) {
			int n = GetRandomValue(1, 100);
			if (n <= 70) {
				std::swap(block[TOP_RIGHT], block[TOP_LEFT]);
			}
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
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		Vector2 mouse_px = GetMousePosition();
		mouse_px = { mouse_px.x - w, mouse_px.y - h };
		// Convert mouse position from pixels to cells
		Vector2 mouse_cell = { floor(mouse_px.x/cell_size), floor(mouse_px.y/cell_size) };
		
		if (int(mouse_cell.y) >= 0 && int(mouse_cell.y) < f_h && int(mouse_cell.x) >= 0 && int(mouse_cell.x) < f_w) {
			field[int(mouse_cell.y)][int(mouse_cell.x)].type = WATER;
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
			/*if (field[y][x].type == EMPTY) {
				DrawRectangleV(convert_to_px(temp_pos), v2c_size, BLACK);
			}*/
			if (field[y][x].type == SAND) {
				DrawRectangleV(convert_to_px(temp_pos), v2c_size, BEIGE);
			}
			if (field[y][x].type == WATER) {
				DrawRectangleV(convert_to_px(temp_pos), v2c_size, BLUE);
			}
		}
	}

	for (int x = 0; x <= f_w; x++) {
		if (x == 0 || x == f_w) {
			DrawLineV(convert_to_px({ float(x), float(0)}), convert_to_px({float(x), float(f_h)}), GRAY);
		}
	}
	for (int y = 0; y <= f_h; y++) {
		if (y == 0 || y == f_h) {
			DrawLineV(convert_to_px({ float(0), float(y)}), convert_to_px({float(f_w), float(y)}), GRAY);
		}
	}

}

void render() {
	BeginDrawing();
	ClearBackground(BLACK);
// start ImGui Conent
		rlImGuiBegin();

		// show ImGui Content
		bool open = true;
		ImGui::ShowDemoWindow(&open);

		open = true;
		if (ImGui::Begin("Test Window", &open))
		{
			ImGui::TextUnformatted(ICON_FA_JEDI);
			ImGui::Text("Welcome to fucking Falling Sands Motherfucker.");
			for (int i = 0; i < 4; i++) {
				ImGui::Text("Is_Liquid: %d", material[i].is_liquid);
			}
		}
		ImGui::End();

		// end ImGui Content
		rlImGuiEnd();

	render_field();
	EndDrawing();
}

int main() {
	InitWindow(scr_w, scr_h, "Falling sand");

	init_field();

	SetTargetFPS(60);
	rlImGuiSetup(true);

	while (!WindowShouldClose()) {
		draw();
		bc_automaton();
		render();
	}
	return 0;
}
