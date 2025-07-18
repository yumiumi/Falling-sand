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

void swap_cell(Cell* block, int a, int b) {
	if (material[block[a].type].is_static == false && material[block[b].type].is_static == false) {
		std::swap(block[a], block[b]);
	}
}

void block_physics(Cell* block) {
	if (material[block[TOP_LEFT].type].density > material[block[BOT_LEFT].type].density) {
		swap_cell(block, TOP_LEFT, BOT_LEFT);
	}
	if (material[block[TOP_RIGHT].type].density > material[block[BOT_RIGHT].type].density) {
		swap_cell(block, TOP_RIGHT, BOT_RIGHT);
	}
	if (material[block[TOP_LEFT].type].density <= material[block[BOT_LEFT].type].density) {
		if (material[block[BOT_RIGHT].type].density < material[block[TOP_LEFT].type].density) {
			swap_cell(block, TOP_LEFT, BOT_RIGHT);
		}
	}
	if (material[block[TOP_RIGHT].type].density <= material[block[BOT_RIGHT].type].density) {
		if (material[block[BOT_LEFT].type].density < material[block[TOP_RIGHT].type].density) {
			swap_cell(block, TOP_RIGHT, BOT_LEFT);
		}
	}

	if (material[block[BOT_LEFT].type].density >= material[block[TOP_LEFT].type].density
		&& material[block[BOT_RIGHT].type].density >= material[block[TOP_LEFT].type].density
		&& material[block[TOP_LEFT].type].is_liquid == true) {
		if (material[block[TOP_RIGHT].type].density < material[block[TOP_LEFT].type].density) {
			int n = GetRandomValue(1, 100);
			if (n <= 70) {
				swap_cell(block, TOP_LEFT, TOP_RIGHT);
			}
		}
	}
	else if (material[block[BOT_RIGHT].type].density >= material[block[TOP_RIGHT].type].density
		&& material[block[BOT_LEFT].type].density >= material[block[TOP_RIGHT].type].density
		&& material[block[TOP_RIGHT].type].is_liquid == true) {
		if (material[block[TOP_LEFT].type].density < material[block[TOP_RIGHT].type].density) {
			int n = GetRandomValue(1, 100);
			if (n <= 70) {
				swap_cell(block,TOP_RIGHT, TOP_LEFT);
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
	// show ImGui Content
	bool open = true;

	ImGui::ShowDemoWindow(&open);

	static Cell_Type temp_type;

	open = true;
	if (ImGui::Begin("Test Window", &open))
	{
		// Buttons colors
		float max_color = 4.f;
        for (int i = 0; i < max_color; i++)
        {
			if (i > 0) {
				ImGui::SameLine();
			}
			if (i == 0) {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(80, 80, 80, 255)); // DARKGRAY
				if (ImGui::Button("Border")) {
					temp_type = BORDER;
				}
			}
			if (i == 1) {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(130, 130, 130, 255)); // GRAY
				if (ImGui::Button("Empty")) {
					temp_type = EMPTY;
				}
			}
			if (i == 2) {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(211, 176, 131, 255)); // BEIGE
				if (ImGui::Button("Sand")) {
					temp_type = SAND;
				}
			}
			if (i == 3) {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(0, 121, 241, 255)); // BLUE
				if (ImGui::Button("Water")) {
					temp_type = WATER;
				}
			}
            ImGui::PopStyleColor(1);
        }
	}
	ImGui::End();

	float w = scr_w/2 - f_w/2 * cell_size;
	float h = scr_h/2 - f_h/2 * cell_size;

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		Vector2 mouse_px = GetMousePosition();
		mouse_px = { mouse_px.x - w, mouse_px.y - h };
		// Convert mouse position from pixels to cells
		Vector2 mouse_cell = { floor(mouse_px.x/cell_size), floor(mouse_px.y/cell_size) };
		
		if (int(mouse_cell.y) >= 0 && int(mouse_cell.y) < f_h && int(mouse_cell.x) >= 0 && int(mouse_cell.x) < f_w) {
			if (temp_type == EMPTY && field[int(mouse_cell.y)][int(mouse_cell.x)].type != EMPTY) {
				field[int(mouse_cell.y)][int(mouse_cell.x)].type = temp_type;
			}
			else if (temp_type != EMPTY && field[int(mouse_cell.y)][int(mouse_cell.x)].type == EMPTY) {
				field[int(mouse_cell.y)][int(mouse_cell.x)].type = temp_type;
			}
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
			if (field[y][x].type == SAND) {
				DrawRectangleV(convert_to_px(temp_pos), v2c_size, BEIGE);
			}
			if (field[y][x].type == WATER) {
				DrawRectangleV(convert_to_px(temp_pos), v2c_size, BLUE);
			}
			if (field[y][x].type == BORDER) {
				DrawRectangleV(convert_to_px(temp_pos), v2c_size, DARKGRAY);
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
	BeginDrawing();
	ClearBackground(BLACK);
		// start ImGui Conent
		rlImGuiBegin();

		draw();
		bc_automaton();
		render();
	}
	return 0;
}
