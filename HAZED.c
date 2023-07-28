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

    Cube testCube;

    Camera playerCamera = {
        .cameraPos = { 0.0f, 0.0f, -5.0f },
        .cameraRot = { 0.0f, 0.0f, 0.0f },
        .displayPlane = { 0.0f, 0.0f, 1.0f }
    };


    setCubePoints(&testCube);

    while (!quit) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            handleInput(&playerCamera, &event);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int i = 0; i < 8; i++) {

            testCube.distanceToCamera[i].x = testCube.points[i].x - playerCamera.cameraPos.x;
            testCube.distanceToCamera[i].y = testCube.points[i].y - playerCamera.cameraPos.y;
            testCube.distanceToCamera[i].z = testCube.points[i].z - playerCamera.cameraPos.z;
        }

        projectAndRender(renderer, &playerCamera, &testCube, WINDOW_WIDTH, WINDOW_HEIGHT);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}