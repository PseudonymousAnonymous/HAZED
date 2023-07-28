bool mouseFirst = true;
bool mouseMotion = false;

float mouseRelX, mouseRelY;

const float sensitivity = 0.001f;

float convertAngle(float angle) {
    return fmodf(fmodf(angle, 360.0f) + 360.0f, 360.0f);
}

void handleInput(Camera* playerCamera, SDL_Event* event) {
        if (event->type == SDL_KEYDOWN) {
            switch (event->key.keysym.sym) {
            case SDLK_SPACE:
                playerCamera->cameraPos.y += 0.1f;
                break;

            case SDLK_LSHIFT:
                playerCamera->cameraPos.y -= 0.1f;
                break;

            case SDLK_a:
                playerCamera->cameraPos.x -= 0.1f;
                break;

            case SDLK_d:
                playerCamera->cameraPos.x += 0.1f;
                break;

            case SDLK_s:
                playerCamera->cameraPos.z -= 0.1f;
                break;

            case SDLK_w:
                playerCamera->cameraPos.z += 0.1f;
                break;

            default:
                break;
            }
        }
        if (event->type == SDL_MOUSEMOTION) {

            mouseMotion = true;

            //playerCamera->cameraRot.x = convertAngle(playerCamera->cameraRot.x);
            //playerCamera->cameraRot.y = convertAngle(playerCamera->cameraRot.y);

            printf("Angles (X, Y) %f, %f \n", playerCamera->cameraRot.x, playerCamera->cameraRot.y);

            if (!mouseFirst) {
                mouseRelX = event->motion.xrel;
                mouseRelY = event->motion.yrel;
            }
            else {
                mouseFirst = false;
                mouseRelX = 0;
                mouseRelY = 0;
            }
            if (mouseMotion) {
                mouseMotion = false;
                playerCamera->cameraRot.y += mouseRelX * sensitivity;
                playerCamera->cameraRot.x -= mouseRelY * sensitivity;
            }
        }
}
