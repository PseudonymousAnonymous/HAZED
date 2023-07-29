typedef struct {
    int x, y;
}VECT2D;

typedef struct {
    float x, y, z;
}VECT3D;

typedef struct {
    VECT3D* points;
    VECT3D* distanceToCamera;
    int numVertices;
}Object;

typedef struct {
    VECT3D cameraPos;
    VECT3D cameraRot;
    VECT3D displayPlane;
    float neededYPos;
}Camera;