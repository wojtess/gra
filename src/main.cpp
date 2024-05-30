#include "game.h"

int main(void)
{
    Game g;
    g.setScreen(std::make_unique<Screen::HomeScreen>());
    g.run();
    return 0;
}
