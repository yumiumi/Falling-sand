#pragma once

enum Cell_Type {
	EMPTY,
	SAND,
	BORDER,
	WATER,
};

const int f_w = 70; // 160
const int f_h = 70;

const int cell_size = 7; // 4
const int grid_thickness = 1;

struct Cell {
	Color color; 
	Cell_Type type;
	bool is_updated = false;
};

Cell field[f_h][f_w];
