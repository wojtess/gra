#pragma once
#include "screen.h"
#include "renderer.h"
#include <memory>

class Game {
    private:
    std::unique_ptr<Screen::AbstractScreen> screen;
    public:
    void setScreen(std::unique_ptr<Screen::AbstractScreen>);

    void run();
};