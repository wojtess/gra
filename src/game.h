#pragma once
#include "screen.h"
#include "renderer.h"
#include "object.h"
#include <memory>

class Game {
    private:
    std::unique_ptr<Screen::AbstractScreen> screen;
    std::vector<std::shared_ptr<PhysicsObject>> entitys;
    std::shared_ptr<Entity::Player> player;

    public:
    void setScreen(std::unique_ptr<Screen::AbstractScreen>);
    void run();
    void newGame();
    std::vector<std::shared_ptr<PhysicsObject>> getEntitys() const;
    std::shared_ptr<Entity::Player> getPlayer() const;
};