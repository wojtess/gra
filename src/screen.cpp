#include "screen.h"
#include "object.h"
#include "game.h"
#include "rlgl.h"

namespace Screen {
    void HomeScreen::render(Game& game) {
        auto width = GetScreenWidth();
        auto height = GetScreenHeight();
        
        Hud::Button buttonExit([]() {
            exit(0);
        }, "Exit", {20, 20}, {200, 100}, 20);

        Hud::Button buttonPlay([&game]() {
            game.setScreen(std::make_unique<Screen::GameScreen>());
        }, "Play", {100, 100}, {200, 100}, 20);

        Hud::Label name("nazwa gry", {width / 2.0, 100}, Renderer::DEFAULT_TEHEME.getTextColor(), 20);

        BeginDrawing();
        ClearBackground(Color{187, 205, 229, 255});

        name.render();
        buttonExit.render();
        buttonPlay.render();
        
        EndDrawing();
    }

    void GameScreen::render(Game& game) {
        // {
        //     auto cameraLock = renderer.get2DModeLock();
        //     auto it = objects.begin();
        //     while(it != objects.end()) {
        //         if(auto renderable = it->lock()) {
        //             // rlPushMatrix();
        //             // rlTranslatef(renderable->getPos().x, renderable->getPos().y, 0);
        //             renderable->render(renderer);
        //             // rlPopMatrix();
        //         } else {
        //             objects.erase(it);
        //         }
        //     }
        // }
        
        // {
        //     auto it = hud.begin();
        //     while(it != hud.end()) {
        //         if(auto renderable = it->lock()) {
        //             renderable->render(renderer);
        //         } else {
        //             hud.erase(it);
        //         }
        //     }
        // }
        Hud::Button button([&game]() {
            game.setScreen(std::make_unique<Screen::HomeScreen>());
        }, "back", {0,0}, {200, 200}, 20);
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // button.render();

        for(const auto& entity:game.getEntitys()) {
            rlPushMatrix();
                auto pos = entity->getPos();
                rlTranslatef(pos.x, pos.y, 0);
                entity->render();
            rlPopMatrix();
        }

        EndDrawing();
    }

    GameScreen::GameScreen() {

    }
    GameScreen::~GameScreen() {}
}