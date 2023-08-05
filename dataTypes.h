typedef struct {
    int x, y;
    bool willBeDrawn;
}VECT2D;

typedef struct {
    float x, y, z;
}VECT3D;

typedef struct {
    VECT3D* points;
    VECT3D distanceToCamera[3];
    int numVertices;
}Object;

typedef struct {
    VECT3D cameraPos;
    VECT3D cameraRot;
    VECT3D displayPlane;
    float neededYPos;
}Camera;

typedef struct {
    int red, green, blue;
}Color;
