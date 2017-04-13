

#include "grid.h"

Grid::Grid(int w, int h) {
	width = w;
	height = h;
	for (int i = 0; i < width; i++) {
		vector<Tile*> newRow;
		for (int j = 0; j < height; j++) {
			Tile* toAdd;
			if (i == 0 || i == width - 1 || j == 0 || j == height-1) {
				toAdd = new WallTile(i, j);
			}
			else {
				toAdd = new FloorTile(i, j);
			}
			
			newRow.push_back(toAdd);
		}
		grid.push_back(newRow);
	}
}

Grid::~Grid() {}

