#pragma once

enum Cell_Type {
	EMPTY,
	SAND,
	BORDER,
	WATER,
};

const int f_w = 160; // 160
const int f_h = 120;

const int cell_size = 4; // 4
const int grid_thickness = 1;

struct Cell_Material {
	float density;
	bool is_liquid = false;
};

Cell_Material material[4] = {
	{
		// material[0] = EMPTY
		.density = 0.f,
	},
	{
		// material[1] = SAND
		.density = 1.5f,
	},
	{
		// material[2] = BORDER
		.density = 999.f,
	},
	{
		// material[3] = WATER
		.density = 1.f,
		.is_liquid = true,
	},
};

struct Cell {
	Color color; 
	Cell_Type type;
	bool is_updated = false;
};

Cell field[f_h][f_w];
