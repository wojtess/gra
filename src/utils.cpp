#include "utils.h"
#include "raymath.h"

namespace RayCastUtils {
    std::optional<Vector2> intersectionLine(Vector2 rayOrigin, Vector2 rayDirection, Vector2 lineStart, Vector2 lineEnd) {
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

    std::optional<Vector2> intersectionCircle(Vector2 rayOrigin, Vector2 rayDirection, Vector2 center, float r) {
        // Calculate the vector from the ray origin to the circle center
        Vector2 oc = Vector2Subtract(center, rayOrigin);
        
        // Calculate projection of oc onto the ray direction
        float t = Vector2DotProduct(oc, rayDirection);
        
        // Calculate the point on the ray closest to the circle center
        Vector2 closestPoint = Vector2Add(rayOrigin, Vector2Scale(rayDirection, t));
        
        // Calculate the distance from the closest point to the circle center
        float d = Vector2DistanceSqr(closestPoint, center);
        float rsq = r * r;
        
        // Check if the closest point is within the circle
        if (d > rsq) {
            return {};
        }
        
        // Calculate the distance from the closest point to the intersection points
        float thc = sqrtf(rsq - d);
        
        // Calculate intersection points (only the closest one is needed)
        float t0 = t - thc;
        float t1 = t + thc;
        
        // Ensure the intersection point is in front of the ray origin
        if (t0 >= 0) {
            return Vector2Add(rayOrigin, Vector2Scale(rayDirection, t0));
        } else if (t1 >= 0) {
            return Vector2Add(rayOrigin, Vector2Scale(rayDirection, t1));
        } else {
            return {};
        }
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