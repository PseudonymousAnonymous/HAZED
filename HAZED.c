#include "HAZED.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

float D_X[8];
float D_Y[8];
float D_Z[8];

float B_X[8];
float B_Y[8];

int main() {
    Cube testCube;
    Camera playerCamera;

    setCubePoints(&testCube);

    // Initialize playerCamera to default values
    playerCamera.cameraPos.x = 0.0;
    playerCamera.cameraPos.y = -5.0;
    playerCamera.cameraPos.z = 0.0;

    playerCamera.cameraRot.x = 0.0;
    playerCamera.cameraRot.y = 0.0;
    playerCamera.cameraRot.z = 0.0;

    playerCamera.displayPlane.x = 0.0;
    playerCamera.displayPlane.y = 2.0;
    playerCamera.displayPlane.z = 0.0;

    for (int i = 0; i < 8; i++) {
        testCube.distanceToCamera[i].x = testCube.points[i].x - playerCamera.cameraPos.x;
        testCube.distanceToCamera[i].y = testCube.points[i].y - playerCamera.cameraPos.y;
        testCube.distanceToCamera[i].z = testCube.points[i].z - playerCamera.cameraPos.z;

        float cx = cosf(playerCamera.cameraRot.x);
        float cy = cosf(playerCamera.cameraRot.y);
        float cz = cosf(playerCamera.cameraRot.z);

        float sx = sinf(playerCamera.cameraRot.x);
        float sy = sinf(playerCamera.cameraRot.y);
        float sz = sinf(playerCamera.cameraRot.z);

        float X = testCube.distanceToCamera[i].x;
        float Y = testCube.distanceToCamera[i].y;
        float Z = testCube.distanceToCamera[i].z;

        float ex = playerCamera.displayPlane.x;
        float ey = playerCamera.displayPlane.y;
        float ez = playerCamera.displayPlane.z;

        D_X[i] = cy * ((sz * Y) + (cz * X)) - (sy * Z);
        D_Y[i] = sx * ((cy * Z) + (sy * ((sz * Y) + (cz * X)))) + (cx * ((cz * Y) - (sz * X)));
        D_Z[i] = cx * ((cy * Z) + (sy * ((sz * Y) + (cz * X)))) - (sx * ((cz * Y) - (sz * X)));

        if (D_Z[i] == 0) {
            D_Z[i] += 0.001f;
        }

        B_X[i] = ((ez / D_Z[i]) * D_X[i]) + ex;
        B_Y[i] = ((ez / D_Z[i]) * D_Y[i]) + ey;

        printf("%f \n", B_X[i]);
        printf("%f \n", B_Y[i]);
    }

    return 0;
}