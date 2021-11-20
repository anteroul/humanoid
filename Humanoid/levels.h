#pragma once
#include "arkanoid.h"

int level1[LINES_OF_BRICKS][BRICKS_PER_LINE] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5
};

int level2[LINES_OF_BRICKS][BRICKS_PER_LINE] = {
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5
};

typedef struct Level {
	int brickMap[LINES_OF_BRICKS][BRICKS_PER_LINE];
} Level;

static Level stage[10] = { level1[LINES_OF_BRICKS][BRICKS_PER_LINE], level2[LINES_OF_BRICKS][BRICKS_PER_LINE] };