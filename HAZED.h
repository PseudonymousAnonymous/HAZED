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
#include "convexHull.h"
#include "scene.h"

#define SWAP(x,y) 		do { (x)=(x)^(y); (y)=(x)^(y); (x)=(x)^(y); } while(0)

float D_X[8];
float D_Y[8];
float D_Z[8];

float B_X[8];
float B_Y[8];

float nearClip = 0.01f;
float farClip = 100.0f;

static int width, height;
static Color*** imageData = NULL;



int loadCount(const char* filename) {

    FILE* sceneNumberFile;

    int numberOfScenes;

    errno_t err = fopen_s(&sceneNumberFile, filename, "r");
    if (err != 0) {
        printf("Can't open file for reading.\n");
        return 0;
    }

    fscanf_s(sceneNumberFile, "%d", &numberOfScenes);

    fclose(sceneNumberFile);

    return numberOfScenes;
}




int loadSkyImage(const char* filename) {
    FILE* skyImage;

    errno_t err = fopen_s(&skyImage, filename, "r");
    if (err != 0) {
        printf("Can't open file for reading.\n");
        return 0;
    }

    if (fscanf_s(skyImage, "%d %d", &width, &height) != 2) {
        printf("Error reading image dimensions.\n");
        fclose(skyImage);
        return 0;
    }

    imageData = (Color***)malloc(height * sizeof(Color**));
    for (int i = 0; i < height; i++) {
        imageData[i] = (Color**)malloc(width * sizeof(Color*));
        for (int j = 0; j < width; j++) {
            imageData[i][j] = (Color*)malloc(sizeof(Color));
            if (fscanf_s(skyImage, "%d %d %d", &imageData[i][j]->red, &imageData[i][j]->green, &imageData[i][j]->blue) != 3) {
                printf("Error reading pixel data.\n");
                fclose(skyImage);
                for (int a = 0; a <= i; a++) {
                    for (int b = 0; b < width; b++)
                        free(imageData[a][b]);
                    free(imageData[a]);
                }
                free(imageData);
                return 0;
            }
        }
    }

    fclose(skyImage);
    return 1;
}

void drawBackground(SDL_Renderer* renderer) {
    if (imageData == NULL) {
        printf("No image data available.\n");
        return;
    }

    float scaleX = (float)WINDOW_WIDTH / width;
    float scaleY = (float)WINDOW_HEIGHT / height;

    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            int imageX = (int)(x / scaleX);
            int imageY = (int)(y / scaleY);

            if (imageX >= 0 && imageX < width && imageY >= 0 && imageY < height) {
                SDL_SetRenderDrawColor(renderer, imageData[imageY][imageX]->red, imageData[imageY][imageX]->green, imageData[imageY][imageX]->blue, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}

void freeImageData() {

    if (imageData != NULL) {

        for (int i = 0; i < height; i++) {

            for (int j = 0; j < width; j++)
                free(imageData[i][j]);
            free(imageData[i]);
        }
        free(imageData);

        imageData = NULL;
    }
}

void fillTriangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3) {
    int minX = (x1 < x2) ? ((x1 < x3) ? x1 : x3) : ((x2 < x3) ? x2 : x3);
    int minY = (y1 < y2) ? ((y1 < y3) ? y1 : y3) : ((y2 < y3) ? y2 : y3);
    int maxX = (x1 > x2) ? ((x1 > x3) ? x1 : x3) : ((x2 > x3) ? x2 : x3);
    int maxY = (y1 > y2) ? ((y1 > y3) ? y1 : y3) : ((y2 > y3) ? y2 : y3);

    printf("Points: %d %d %d %d %d %d\n", x1, y1, x2, y2, x3, y3);

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            int w1 = (x2 - x1) * (y - y1) - (y2 - y1) * (x - x1);
            int w2 = (x3 - x2) * (y - y2) - (y3 - y2) * (x - x2);
            int w3 = (x1 - x3) * (y - y3) - (y1 - y3) * (x - x3);

            if (w1 >= 0 && w2 >= 0 && w3 >= 0) {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}


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

    Color fillColor = {
        .red = 255,
        .blue = 0,
        .green = 0
    };

    for (int objIndex = 0; objIndex < numObjects; objIndex++) {
        const Object* currentObject = &objects[objIndex];

        if (currentObject->numVertices <= 0) {
            continue;
        }

        VECT2D* projectedPoints = (VECT2D*)malloc(currentObject->numVertices * sizeof(VECT2D));

        for (int vertexIndex = 0; vertexIndex < currentObject->numVertices; vertexIndex++) {

            float X = currentObject->distanceToCamera[vertexIndex].x;
            float Y = currentObject->distanceToCamera[vertexIndex].y;
            float Z = currentObject->distanceToCamera[vertexIndex].z;

            float rotatedX = cy * (sz * Y + cz * X) - sy * Z;
            float rotatedY = sx * (cy * Z + sy * (sz * Y + cz * X)) + cx * (cz * Y - sz * X);
            float rotatedZ = cx * (cy * Z + sy * (sz * Y + cz * X)) - sx * (cz * Y - sz * X);

            float projectedX = (ez / rotatedZ) * rotatedX + ex;
            float projectedY = (ez / rotatedZ) * rotatedY + ey;

            int screenX = (int)(projectedX * (WINDOW_WIDTH / 2)) + (WINDOW_WIDTH / 2);
            int screenY = (int)(projectedY * (WINDOW_HEIGHT / 2)) + (WINDOW_HEIGHT / 2);

            projectedPoints[vertexIndex].x = screenX;
            projectedPoints[vertexIndex].y = screenY;

            // Clip against near and far planes (optional, but recommended for correctness)
            if (rotatedZ <= nearClip || rotatedZ >= farClip) {
                projectedPoints[vertexIndex].willBeDrawn = false;
            }
            else {
                projectedPoints[vertexIndex].willBeDrawn = true;
            }
        }

        // Draw wireframe model
        for (int k = 0; k < currentObject->numVertices; k++) {
            int nextIndex = (k + 1) % currentObject->numVertices;
            if (projectedPoints[k].willBeDrawn && projectedPoints[nextIndex].willBeDrawn) {
                SDL_RenderDrawLine(renderer, projectedPoints[k].x, projectedPoints[k].y, projectedPoints[nextIndex].x, projectedPoints[nextIndex].y);
            }
        }

        // Fill the triangle
        if (currentObject->numVertices >= 3) {
        //    fillTriangle(renderer, projectedPoints[0].x, projectedPoints[0].y, projectedPoints[1].x, projectedPoints[1].y, projectedPoints[2].x, projectedPoints[2].y);
        }
        //fillTriangle(renderer, projectedPoints[0].x, projectedPoints[0].y, projectedPoints[1].x, projectedPoints[1].y, projectedPoints[2].x, projectedPoints[2].y);
        free(projectedPoints);
    }
}


bool loadObjectsFromScene(const char* basePath, int sceneNumber, Object** objects, int* numObjects) {
    // Construct the file path for the scene's .ob file
    char filePath[200];
    sprintf(filePath, "%s/%d/num.txt", basePath, sceneNumber);

    // Load the number of objects in the scene
    *numObjects = loadCount(filePath);

    // Allocate memory for the objects
    *objects = (Object*)malloc((*numObjects) * sizeof(Object));
    if (*objects == NULL) {
        printf("Memory allocation failed for objects.\n");
        return false;
    }

    // Load objects from .ob files
    for (int i = 0; i < *numObjects; i++) {
        sprintf(filePath, "%s/%d/%d.ob", basePath, sceneNumber, i + 1);
        FILE* file = fopen(filePath, "r");
        if (file == NULL) {
            printf("Failed to open file: %s\n", filePath);
            free(*objects); // Free allocated memory before returning
            return false;
        }

        (*objects)[i].numVertices = 3;
        (*objects)[i].points = (VECT3D*)malloc((*objects)[i].numVertices * sizeof(VECT3D)); // Allocate memory for points

        if ((*objects)[i].points == NULL) {
            printf("Memory allocation failed for points in object %d.\n", i);
            for (int j = 0; j < i; j++) {
                free((*objects)[j].points); // Free previously allocated memory
            }
            free(*objects);
            return false;
        }

        for (int k = 0; k < 3; k++) {
            float x, y, z;
            if (fscanf(file, "%f %f %f", &x, &y, &z) != 3) {
                printf("Error reading point %d for object %d\n", k + 1, i + 1);
                fclose(file);
                for (int j = 0; j <= i; j++) {
                    free((*objects)[j].points); // Free allocated memory before returning
                }
                free(*objects);
                return false;
            }

            (*objects)[i].points[k].x = x;
            (*objects)[i].points[k].y = y;
            (*objects)[i].points[k].z = z;
        }

        fclose(file);
    }

    return true;
}