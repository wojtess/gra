#pragma once
#include "renderer.h"
#include <memory>
#include "object.h"

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
    Camera2D camera;
    //copy of pointer to the player, used when game is paused. When game is paused then we set player in game to nullptr
    //sa main loop thinks that there is no world loaded, then when game is resumed we will set player in main loop to this saved value
    std::shared_ptr<Entity::Player> player;
    Vector2 savedMousePos;
    bool paused;

    void pause(Game& game);
    void resume(Game& game);
    public:
    GameScreen();
    void render(Game&) override;
    ~GameScreen();
};

class GameOver: public AbstractScreen {
    private:
    Entity::Player::Stats stats;
    public:
    GameOver(Entity::Player::Stats);
    void render(Game&) override;
};

};
