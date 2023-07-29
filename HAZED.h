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

float nearClip = 0.01f;
float farClip = 100.0f;

void projectAndRender(SDL_Renderer* renderer, const Camera* camera, const Object* objects, int numObjects) {

    float ex = camera->displayPlane.x;
    float ey = camera->displayPlane.y;
    float ez = camera->displayPlane.z;

    // Camera rotations
    float cx = cosf(camera->cameraRot.x);
    float cy = cosf(camera->cameraRot.y);
    float cz = cosf(camera->cameraRot.z);

    float sx = sinf(camera->cameraRot.x);
    float sy = sinf(camera->cameraRot.y);
    float sz = sinf(camera->cameraRot.z);

    for (int objIndex = 0; objIndex < numObjects; objIndex++) {
        const Object* currentObject = &objects[objIndex];

        VECT2D* projectedPoints = (VECT2D*) malloc(currentObject->numVertices * sizeof(VECT2D));

        for (int vertexIndex = 0; vertexIndex < currentObject->numVertices; vertexIndex++) {

            float X = currentObject->distanceToCamera[vertexIndex].x;
            float Y = currentObject->distanceToCamera[vertexIndex].y;
            float Z = currentObject->distanceToCamera[vertexIndex].z;

            float rotatedX = cy * (sz * Y + cz * X) - sy * Z;
            float rotatedY = sx * (cy * Z + sy * (sz * Y + cz * X)) + cx * (cz * Y - sz * X);
            float rotatedZ = cx * (cy * Z + sy * (sz * Y + cz * X)) - sx * (cz * Y - sz * X);

            float projectedX = (ez / rotatedZ) * rotatedX + ex;
            float projectedY = (ez / rotatedZ) * rotatedY + ey;

            // Clipping
            if (rotatedZ <= nearClip || rotatedZ >= farClip) {
                // Skip points that are outside the view frustum
                continue;
            }

            // Convert the 3D coordinates to 2D screen coordinates
            int screenX = (int)(projectedX * (WINDOW_WIDTH / 2)) + (WINDOW_WIDTH / 2);
            int screenY = (int)(projectedY * (WINDOW_HEIGHT / 2)) + (WINDOW_HEIGHT / 2);

            projectedPoints[vertexIndex].x = screenX;
            projectedPoints[vertexIndex].y = screenY;

            SDL_RenderDrawPoint(renderer, screenX, screenY);

            //printf("Vertex Index %d \n", vertexIndex);

            if (projectedPoints == NULL) {
                printf("Memory allocation error for projectedPoints.\n");
                free(projectedPoints);
                return;
            }

            if (vertexIndex > 0) {
                SDL_RenderDrawLine(renderer, projectedPoints[vertexIndex - 1].x, projectedPoints[vertexIndex - 1].y, projectedPoints[vertexIndex].x, projectedPoints[vertexIndex].y);
            }
        }

        free(projectedPoints);
    }
}