#include "game.h"
#include <memory>

void Game::run() {
    std::unique_ptr<Renderer::AbstractRenderer> renderer;
    renderer = std::make_unique<Renderer::RaylibRenderer>("", 1280, 720);

    while(true) {
        renderer->render(objects2D, hud, background);
    }
}