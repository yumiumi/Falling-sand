#pragma once

enum Cell_Type {
	EMPTY,
	SAND,
	BORDER,
};

const int f_w = 80; // 160
const int f_h = 80;

const int cell_size = 8; // 4
const int grid_thickness = 1;

struct Cell {
	Color color; 
	Cell_Type type;
	bool is_updated = false;
};

Cell field[f_h][f_w];
