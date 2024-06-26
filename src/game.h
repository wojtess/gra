#pragma once
#include "screen.h"
#include "renderer.h"
#include "object.h"
#include <memory>
#include "constants.h"

class Game {
    private:
    std::unique_ptr<Screen::AbstractScreen> screen;
    std::vector<std::shared_ptr<PhysicsObject>> entitys;
    std::shared_ptr<Entity::Player> player;
    Renderer::ResourceMap resourceMap;

    public:
    Game();
    void setScreen(std::unique_ptr<Screen::AbstractScreen>);
    void run();
    void newGame();
    std::vector<std::shared_ptr<PhysicsObject>>& getEntitys();
    std::shared_ptr<Entity::Player> getPlayer();
    void setPlayer(std::shared_ptr<Entity::Player>);
    Renderer::ResourceMap& getResourceMap();
};