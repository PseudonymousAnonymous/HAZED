int crossProduct(VECT2D p0, VECT2D p1, VECT2D p2) {
    return ((p1.x - p0.x) * (p2.y - p0.y)) - ((p1.y - p0.y) * (p2.x - p0.x));
}

VECT2D getNextHullPoint(VECT2D points[], int n, VECT2D current) {
    VECT2D next = points[0];
    for (int i = 1; i < n; i++) {
        if (points[i].x == current.x && points[i].y == current.y) {
            continue;
        }
        int val = crossProduct(current, next, points[i]);
        if (val < 0 || (val == 0 && abs(points[i].x - current.x) + abs(points[i].y - current.y) > abs(next.x - current.x) + abs(next.y - current.y))) {
            next = points[i];
        }
    }
    return next;
}

void convexHull(VECT2D points[], int n) {
    if (n < 3) {
        printf("Convex hull not possible with less than 3 points.\n");
        return;
    }

    VECT2D start = points[0];
    for (int i = 1; i < n; i++) {
        if (points[i].x < start.x || (points[i].x == start.x && points[i].y < start.y)) {
            start = points[i];
        }
    }

    VECT2D current = start;
    printf("Convex hull points:\n");

    do {
        printf("(%d, %d)\n", current.x, current.y);
        VECT2D next = getNextHullPoint(points, n, current);
        current = next;
    } while (current.x != start.x || current.y != start.y);
}
