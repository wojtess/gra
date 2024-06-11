#pragma once
#include "raylib.h"
#include <optional>

namespace RayCastUtils {

    std::optional<Vector2> intersectionLine(Vector2 rayOrigin, Vector2 rayDirection, Vector2 lineStart, Vector2 lineEnd);
    std::optional<Vector2> intersectionCircle(Vector2 rayOrigin, Vector2 rayDirection, Vector2 center, float r);

}

bool isInside(Vector2 pos1, Vector2 pos2, Vector2 size);

void DrawTextCenter(const char *text, int posX, int posY, int fontSize, Color color);