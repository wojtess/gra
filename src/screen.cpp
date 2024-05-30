#include "screen.h"
#include "object.h"

namespace Screen {
    void HomeScreen::render(Game& game) {
        Hud::Button button([]() {
            exit(0);
        }, "Exit", {0,0}, {200, 200}, 20);

        BeginDrawing();
        ClearBackground(GRAY);

        button.render();
        
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
        // Hud::Button button([]() {
        //     printf("asd\n");
        // }, "asd", {0,0}, {200, 200}, 20);
        // BeginDrawing();
        // ClearBackground(RAYWHITE);

        // button.render();

        // EndDrawing();
    }

    GameScreen::GameScreen() {

    }
    GameScreen::~GameScreen() {}
}