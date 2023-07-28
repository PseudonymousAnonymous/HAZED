#pragma once

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include "dataTypes.h"
#include "input.h"

float D_X[8];
float D_Y[8];
float D_Z[8];

float B_X[8];
float B_Y[8];

float nearClip = 0.1f;
float farClip = 100.0f;

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

void projectAndRender(SDL_Renderer* renderer, const Camera* camera, const Cube* cube, int windowWidth, int windowHeight) {
    float nearClip = 0.1f;
    float farClip = 100.0f;

    int pointX[8];
    int pointY[8];

    for (int i = 0; i < 8; i++) {
        float X = cube->distanceToCamera[i].x;
        float Y = cube->distanceToCamera[i].y;
        float Z = cube->distanceToCamera[i].z;

        // Apply camera rotations
        float cx = cosf(camera->cameraRot.x);
        float cy = cosf(camera->cameraRot.y);
        float cz = cosf(camera->cameraRot.z);

        float sx = sinf(camera->cameraRot.x);
        float sy = sinf(camera->cameraRot.y);
        float sz = sinf(camera->cameraRot.z);

        float rotatedX = cy * (sz * Y + cz * X) - sy * Z;
        float rotatedY = sx * (cy * Z + sy * (sz * Y + cz * X)) + cx * (cz * Y - sz * X);
        float rotatedZ = cx * (cy * Z + sy * (sz * Y + cz * X)) - sx * (cz * Y - sz * X);

        // Apply perspective projection
        float ex = camera->displayPlane.x;
        float ey = camera->displayPlane.y;
        float ez = camera->displayPlane.z;

        float projectedX = (ez / rotatedZ) * rotatedX + ex;
        float projectedY = (ez / rotatedZ) * rotatedY + ey;

        // Clipping
        if (rotatedZ <= nearClip || rotatedZ >= farClip) {
            // Skip points that are outside the view frustum
            continue;
        }

        // Convert the 3D coordinates to 2D screen coordinates
        pointX[i] = (int)(projectedX * (windowWidth / 2)) + (windowWidth / 2);
        pointY[i] = (int)(projectedY * (windowHeight / 2)) + (windowHeight / 2);

        SDL_RenderDrawPoint(renderer, pointX[i], pointY[i]);
        
        if (i > 0) {
            SDL_RenderDrawLine(renderer, pointX[i - 1], pointY[i - 1], pointX[i], pointY[i]);
        }
    }
}
