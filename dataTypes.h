typedef struct {
    float x, y;
}VECT2D;

typedef struct {
    float x, y, z;
}VECT3D;

typedef struct {
    VECT3D points[8];
    VECT3D distanceToCamera[8];
}Cube;

typedef struct {
    VECT3D cameraPos;
    VECT3D cameraRot;
    VECT3D displayPlane;
}Camera;