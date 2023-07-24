#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>
#include "dataTypes.h"


void setCubePoints(Cube* thisCube) {
	thisCube->points[0].x = 0;
	thisCube->points[0].y = 0;
	thisCube->points[0].z = 0;

	thisCube->points[1].x = 1;
	thisCube->points[1].y = 0;
	thisCube->points[1].z = 0;

	thisCube->points[2].x = 1;
	thisCube->points[2].y = 1;
	thisCube->points[2].z = 0;

	thisCube->points[3].x = 0;
	thisCube->points[3].y = 1;
	thisCube->points[3].z = 0;

	thisCube->points[4].x = 0;
	thisCube->points[4].y = 0;
	thisCube->points[4].z = 1;

	thisCube->points[5].x = 1;
	thisCube->points[5].y = 0;
	thisCube->points[5].z = 1;

	thisCube->points[6].x = 1;
	thisCube->points[6].y = 1;
	thisCube->points[6].z = 1;

	thisCube->points[7].x = 0;
	thisCube->points[7].y = 1;
	thisCube->points[7].z = 1;
}