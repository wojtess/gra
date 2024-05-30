#include "game.h"
#include <memory>

void Game::run() {
    InitWindow(1280, 720, "game");

    while(true) {
        if(this->screen)
            this->screen->render(*this);
    }
}

void Game::setScreen(std::unique_ptr<Screen::AbstractScreen> screen) {
    this->screen = std::move(screen);
}