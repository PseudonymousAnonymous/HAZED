#include "HAZED.h"

int numberOfScenes;


int currentScene = 0;
int numberOfObjects;

char filePathExt[200];
char filePathExtOb[200];

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

    SDL_Surface* pSurface = SDL_GetWindowSurface(window);
    SDL_LockSurface(pSurface);

    Camera playerCamera = {
        .cameraPos = { 0.0f, -1.0f, -5.0f },
        .cameraRot = { 0.0f, 0.0f, 0.0f },
        .displayPlane = { 0.0f, 0.0f, 1.1f },
        .neededYPos = -1.0f
    };

    loadSkyImage("sky.txt");

    numberOfScenes = loadCount("scenes/sceneNum.txt");

    printf("Number of Scenes: %d\n", numberOfScenes);

    sprintf(filePathExt, "scenes/%d/num.txt", currentScene);

    numberOfObjects = loadCount(filePathExt);

    Object* objects = (Object*)malloc(numberOfObjects * sizeof(Object));
    if (objects == NULL) {
        printf("Memory allocation failed for objects.\n");
        return 1;
    }

        printf("Number of Objects in Scene %d: %d\n", currentScene, numberOfObjects);

        if (!loadObjectsFromScene("scenes", currentScene, &objects, &numberOfObjects)) {
            printf("Failed to load objects for Scene %d.\n", currentScene);
            freeImageData();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }

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
                        mouseRelX = (float)event.motion.xrel;
                        mouseRelY = (float)event.motion.yrel;
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

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            //drawBackground(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            for (int i = 0; i < numberOfObjects; i++) {
                for (int j = 0; j < objects[i].numVertices; j++) {
                    objects[i].distanceToCamera[j].x = objects[i].points[j].x - playerCamera.cameraPos.x;
                    objects[i].distanceToCamera[j].y = objects[i].points[j].y - playerCamera.cameraPos.y;
                    objects[i].distanceToCamera[j].z = objects[i].points[j].z - playerCamera.cameraPos.z;
                }
            }

            projectAndRender(renderer, &playerCamera, objects, numberOfObjects);

            SDL_RenderPresent(renderer);
        }

        free(objects);
        freeImageData();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 0;
}