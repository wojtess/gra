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
    
    public:
    GameScreen();
    void render(Game&) override;
    ~GameScreen();
};

};
