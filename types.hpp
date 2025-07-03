#pragma once

enum Cell_Type {
	EMPTY,
	SAND,
};

const int f_w = 60;
const int f_h = 60;

const int cell_size = 10;
const int grid_thickness = 1;

struct Cell {
	Color color; 
	Cell_Type type;
	bool is_updated = false;
};

Cell field[f_h][f_w];
