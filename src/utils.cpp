#include "utils.h"
#include "raymath.h"

namespace RayCastUtils {
    std::optional<Vector2> IntersectionLine(Vector2 rayOrigin, Vector2 rayDirection, Vector2 lineStart, Vector2 lineEnd) {
        // Calculate the direction of the line segment
        Vector2 lineDir = Vector2Subtract(lineEnd, lineStart);

        // Compute the denominator
        float denom = rayDirection.x * lineDir.y - rayDirection.y * lineDir.x;
        if (denom == 0.0f) return {}; // Parallel lines

        // Compute the numerator
        Vector2 diff = Vector2Subtract(lineStart, rayOrigin);
        float t1 = (diff.x * lineDir.y - diff.y * lineDir.x) / denom;
        float t2 = (diff.x * rayDirection.y - diff.y * rayDirection.x) / denom;

        // Check if the intersection point is on the ray and the line segment
        if (t1 >= 0 && t2 >= 0 && t2 <= 1) {
            return Vector2Add(rayOrigin, Vector2Scale(rayDirection, t1));
        }

        return {};
    }
}

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