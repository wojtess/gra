#pragma once
#include "raylib.h"
#include <optional>

namespace RayCastUtils {
    
    std::optional<Vector2> IntersectionLine(Vector2 rayOrigin, Vector2 rayDirection, Vector2 lineStart, Vector2 lineEnd);

}

bool isInside(Vector2 pos1, Vector2 pos2, Vector2 size);