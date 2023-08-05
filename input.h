bool mouseFirst = true;
bool mouseMotion = false;

float mouseRelX, mouseRelY;

const float epsilon = 0.001f;

const float sensitivity = 0.001f;

const float forwardMovement = 0.01f;

const float lateralMovement = 0.01f;

float velocity = 0;

float gravity = 0.001;
float floatyness = 0.025;

bool isJumping = false;

float convertToPositiveAngle(float angle) {
    const float twoPi = 2.0f * M_PI;
    return fmodf(fmodf(angle, twoPi) + twoPi, twoPi);
}

void handleInput(Camera* playerCamera, SDL_Event* event) {

    //printf("Camera Y %f \n", playerCamera->cameraPos.y);

    if (isJumping == true) {
        if (velocity < 0) {
            playerCamera->cameraPos.y += velocity;
            velocity += gravity;
        }
        if (fabs(velocity) < epsilon && fabs(playerCamera->cameraPos.y - playerCamera->neededYPos) > epsilon) {
            playerCamera->cameraPos.y += floatyness;
        }
        if (fabs(velocity) < epsilon && fabs(playerCamera->cameraPos.y - playerCamera->neededYPos) <= epsilon) {
            isJumping = false;
        }
    }
    if (isJumping == false) {
        playerCamera->cameraPos.y = playerCamera->neededYPos;
    }
    
    const Uint8* keys = SDL_GetKeyboardState(NULL);

            if (keys[SDL_SCANCODE_A]) {
                float cosYaw = cosf(playerCamera->cameraRot.y);
                float sinYaw = sinf(playerCamera->cameraRot.y);
                playerCamera->cameraPos.x -= lateralMovement * cosYaw;
                playerCamera->cameraPos.z += lateralMovement * sinYaw;
            }

            if (keys[SDL_SCANCODE_D]) {
                float cosYaw = cosf(playerCamera->cameraRot.y);
                float sinYaw = sinf(playerCamera->cameraRot.y);
                playerCamera->cameraPos.x += lateralMovement * cosYaw;
                playerCamera->cameraPos.z -= lateralMovement * sinYaw;
            }

            if (keys[SDL_SCANCODE_S]) {
                float cosYaw = cosf(playerCamera->cameraRot.y);
                float sinYaw = sinf(playerCamera->cameraRot.y);
                playerCamera->cameraPos.x -= forwardMovement * sinYaw;
                playerCamera->cameraPos.z -= forwardMovement * cosYaw;
            }

            if (keys[SDL_SCANCODE_W]) {
                float cosYaw = cosf(playerCamera->cameraRot.y);
                float sinYaw = sinf(playerCamera->cameraRot.y);
                playerCamera->cameraPos.x += forwardMovement * sinYaw;
                playerCamera->cameraPos.z += forwardMovement * cosYaw;
            }

            if (keys[SDL_SCANCODE_SPACE]) {

                if (isJumping == false) {
                    isJumping = true;
                    velocity = -0.125;
                }
            }

            //printf("POS (X, Y, Z) %f, %f, %f \n", playerCamera->cameraPos.x, playerCamera->cameraPos.y, playerCamera->cameraPos.z);
}
