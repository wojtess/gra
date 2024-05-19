#include "renderer.h"

class Game {
    private:
    Renderer::Renderable2DList objects2D;
    Renderer::RenderableHudList hud;
    std::unique_ptr<Renderer::Renderable> background;
    
    public:
    void run();
};