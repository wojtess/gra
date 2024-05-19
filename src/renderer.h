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


using Renderable2DList = std::vector<std::weak_ptr<Renderable2DObject>>;
using RenderableHudList = std::vector<std::weak_ptr<Renderable>>;

class AbstractRenderer {
    public:
    virtual void render(Renderable2DList&, RenderableHudList&, std::unique_ptr<Renderable>&) = 0;
    virtual void setFPS(int) = 0;
    virtual ~AbstractRenderer() = default;
};

class RaylibRenderer: public AbstractRenderer {
    public:
    Camera2D camera;
    RaylibRenderer(std::string title, int width, int height);
    void render(Renderable2DList&, RenderableHudList&, std::unique_ptr<Renderable>&) override;
    virtual void setFPS(int) override;
    ~RaylibRenderer() override;
};

}

