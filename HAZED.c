#include "HAZED.h"

int main(void) {

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    int quit = 0;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    Object objects[2];

    VECT3D cubeVertices[8] = {
        {0, 0, 0},
        {1, 0, 0},
        {1, 1, 0},
        {0, 1, 0},
        {0, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 1, 1}
    };

    VECT3D cubeDistances[8]; // Initialized later
    objects[0].points = cubeVertices;
    objects[0].distanceToCamera = cubeDistances;
    objects[0].numVertices = 8;

    // Object 2 - A pyramid
    VECT3D pyramidVertices[5] = {
        {0.5, -2, 0.5},
        {0, -3, 0},
        {1, -3, 0},
        {1, -3, 1},
        {0, -3, 1}
    };

    VECT3D pyramidDistances[5]; // Initialized later
    objects[1].points = pyramidVertices;
    objects[1].distanceToCamera = pyramidDistances;
    objects[1].numVertices = 5;


    Camera playerCamera = {
        .cameraPos = { 0.0f, 0.0f, -5.0f },
        .cameraRot = { 0.0f, 0.0f, 0.0f },
        .displayPlane = { 0.0f, 0.0f, 1.1f },
        .neededYPos = 0.0f
    };

    while (!quit) {

        playerCamera.cameraRot.x = convertToPositiveAngle(playerCamera.cameraRot.x);
        playerCamera.cameraRot.y = convertToPositiveAngle(playerCamera.cameraRot.y);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            if (event.type == SDL_MOUSEMOTION) {

                mouseMotion = true;

                //printf("Angles (X, Y) %f, %f \n", playerCamera.cameraRot.x, playerCamera.cameraRot.y);

                if (!mouseFirst) {
                    mouseRelX = event.motion.xrel;
                    mouseRelY = event.motion.yrel;
                }
                else {
                    mouseFirst = false;
                    mouseRelX = 0;
                    mouseRelY = 0;
                }
                if (mouseMotion) {
                    mouseMotion = false;
                    playerCamera.cameraRot.y += mouseRelX * sensitivity;
                    playerCamera.cameraRot.x -= mouseRelY * sensitivity;
                }
            }
        }

        handleInput(&playerCamera, &event);

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < objects[i].numVertices; j++) {
                objects[i].distanceToCamera[j].x = objects[i].points[j].x - playerCamera.cameraPos.x;
                objects[i].distanceToCamera[j].y = objects[i].points[j].y - playerCamera.cameraPos.y;
                objects[i].distanceToCamera[j].z = objects[i].points[j].z - playerCamera.cameraPos.z;
            }
        }

        projectAndRender(renderer, &playerCamera, objects, 2);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}