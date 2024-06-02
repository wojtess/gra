#pragma once
#include "raylib.h"
#include <vector>
#include <memory>
#include <string>

namespace Renderer {

class Renderable {
    public:
    virtual void render() = 0;
    virtual ~Renderable() = default;
};

class Renderable2DObject: public Renderable {
    protected:
    Vector2 pos;
    public:
    Vector2 getPos() const;
    virtual ~Renderable2DObject() = default;
};

class Theme {
    protected:
        Color hover;
        Color bg;
        Color textColor;
    public:
        Theme(Color hover, Color bg, Color textColor);
        Color getHover() const;
        Color getBg() const;
        Color getTextColor() const;
};

static Theme DEFAULT_TEHEME = {
    {59, 131, 145, 255}, {99, 159, 171, 255}, {28, 93, 153, 255}
};

};

