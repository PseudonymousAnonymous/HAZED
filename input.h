bool mouseFirst = true;
bool mouseMotion = false;

float mouseRelX, mouseRelY;

const float sensitivity = 0.001f;

float convertToPositiveAngle(float angle) {
    const float twoPi = 2.0f * M_PI;
    return fmodf(fmodf(angle, twoPi) + twoPi, twoPi);
}

void handleInput(Camera* playerCamera, SDL_Event* event) {
    
    const Uint8* keys = SDL_GetKeyboardState(NULL);

            if (keys[SDL_SCANCODE_A]) {
                float lateralMovement = 0.1f;
                float cosYaw = cosf(playerCamera->cameraRot.y);
                float sinYaw = sinf(playerCamera->cameraRot.y);
                playerCamera->cameraPos.x -= lateralMovement * cosYaw;
                playerCamera->cameraPos.z += lateralMovement * sinYaw;
            }

            if (keys[SDL_SCANCODE_D]) {
                float lateralMovement = 0.1f;
                float cosYaw = cosf(playerCamera->cameraRot.y);
                float sinYaw = sinf(playerCamera->cameraRot.y);
                playerCamera->cameraPos.x += lateralMovement * cosYaw;
                playerCamera->cameraPos.z -= lateralMovement * sinYaw;
            }

            if (keys[SDL_SCANCODE_S]) {
                float forwardMovement = 0.1f;
                float cosYaw = cosf(playerCamera->cameraRot.y);
                float sinYaw = sinf(playerCamera->cameraRot.y);
                playerCamera->cameraPos.x -= forwardMovement * sinYaw;
                playerCamera->cameraPos.z -= forwardMovement * cosYaw;
            }

            if (keys[SDL_SCANCODE_W]) {
                float forwardMovement = 0.1f;
                float cosYaw = cosf(playerCamera->cameraRot.y);
                float sinYaw = sinf(playerCamera->cameraRot.y);
                playerCamera->cameraPos.x += forwardMovement * sinYaw;
                playerCamera->cameraPos.z += forwardMovement * cosYaw;
            }

            printf("POS (X, Y, Z) %f, %f, %f \n", playerCamera->cameraPos.x, playerCamera->cameraPos.y, playerCamera->cameraPos.z);
}
