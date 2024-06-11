#pragma once
#include "raylib.h"
#include <vector>
#include <memory>
#include <string>
#include <map>
#include "raylib.h"

namespace Renderer {


class ResourceMap {
    private:
    std::string texturePath;
    std::map<std::string, Texture2D> textures;
    public:
    //texture path, ex: "path/to/texture"
    ResourceMap(std::string texturePath);
    Texture2D getTexture(std::string name);
    ~ResourceMap();
};

class Renderable {
    public:
    virtual void render(ResourceMap& resourceMap, bool flipped = false, bool effects = false) = 0;
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

