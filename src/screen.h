#pragma once
#include "renderer.h"
#include <memory>

class Game;//forward declaration

namespace Screen {

class AbstractScreen {
    public:
    virtual void render(Game&) = 0;
    virtual ~AbstractScreen() = default;
};

class HomeScreen: public AbstractScreen {
    public:
    void render(Game&) override;
};

class GameScreen: public AbstractScreen {
    private:
    //using std::weak_ptr, list have pointers that when goes out of scope are removed when trying to render them in RaylibRenderer
    using Renderable2DList = std::vector<std::weak_ptr<Renderer::Renderable2DObject>>;
    using RenderableHudList = std::vector<std::weak_ptr<Renderer::Renderable>>;
    //save objects that are renderer, they are on heap so it is better to save list than creating new entries every frame
    Renderable2DList objects;
    RenderableHudList hud;
    std::unique_ptr<Renderer::Renderable> background;
    public:
    GameScreen();
    void render(Game&) override;
    ~GameScreen();
};

};
