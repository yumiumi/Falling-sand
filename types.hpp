#pragma once

enum Cell_Type {
	BORDER,
	EMPTY,
	SAND,
	WATER,
};

const int f_w = 200; // 160
const int f_h = 140;

const int cell_size = 5; // 4
const int grid_thickness = 1;

struct Cell_Material {
	float density;
	bool is_liquid = false;
	bool is_static = false;
};

Cell_Material material[4] = {
	{
		// material[0] = BORDER
		.density = 999.f,
		.is_static = true,
	},
	{
		// material[1] = EMPTY
		.density = 0.f,
	},
	{
		// material[2] = SAND
		.density = 1.5f,
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
