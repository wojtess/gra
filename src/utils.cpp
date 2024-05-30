#include "utils.h"

bool isInside(Vector2 pos1, Vector2 pos2, Vector2 size) {
    // Calculate the boundaries of the rectangle
    float left = pos2.x;
    float right = pos2.x + size.x;
    float top = pos2.y;
    float bottom = pos2.y + size.y;

    // Check if pos1 is within the rectangle
    return (pos1.x >= left && pos1.x <= right &&
            pos1.y >= top && pos1.y <= bottom);
}