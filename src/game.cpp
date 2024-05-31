#include "game.h"
#include <memory>

void Game::run() {
    InitWindow(1280, 720, "game");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    entitys.push_back(std::static_pointer_cast<PhysicsObject>(std::make_shared<Entity::Zombie>(Vector2{10.0f, 10.0f})));

    while(true) {
        if(this->screen)
            this->screen->render(*this);
    }
}

void Game::setScreen(std::unique_ptr<Screen::AbstractScreen> screen) {
    this->screen = std::move(screen);
}

std::vector<std::shared_ptr<PhysicsObject>> Game::getEntitys() const {
    return entitys;
}