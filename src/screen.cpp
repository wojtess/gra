#include "screen.h"
#include "object.h"
#include "game.h"
#include "rlgl.h"
#include "raymath.h"

namespace Screen {
    void HomeScreen::render(Game& game) {
        auto width = GetScreenWidth();
        auto height = GetScreenHeight();
        
        Hud::Button buttonExit([]() {
            exit(0);
        }, "Exit", {20, 20}, {200, 100}, 20);

        Hud::Button buttonPlay([&game]() {
            //START GAME
            game.newGame();
            game.setScreen(std::make_unique<Screen::GameScreen>());
        }, "Play", {100, 100}, {200, 100}, 20);

        Hud::Label name("nazwa gry", {width / 2.0f, 100}, Renderer::DEFAULT_TEHEME.getTextColor(), 20);

        name.render();
        buttonExit.render();
        buttonPlay.render();
    }

    void GameScreen::render(Game& game) {
        if(!game.getPlayer()) {
            //player is nullptr this means that somethink broke
            printf("player is null\n");
            return;
        }

        auto width = GetScreenWidth();
        auto height = GetScreenHeight();

        Hud::Button button([&game]() {
            game.setScreen(std::make_unique<Screen::HomeScreen>());
        }, "back", {0,0}, {200, 200}, 20);

        // button.render();
        camera.offset = Vector2{width / 2.0f, height / 2.0f};
        camera.target = game.getPlayer()->getPos();


        auto mousePos = Vector2Add(game.getPlayer()->getPos(), Vector2Scale(Vector2Subtract(GetMousePosition(), camera.offset), 1.0f/camera.zoom));
        struct rayCastData_t {
            std::shared_ptr<PhysicsObject> object;
            Vector2 pos;
        };
        std::optional<rayCastData_t> objectOnCoursor;
        {
            auto origin = game.getPlayer()->getPos();

            float len = std::numeric_limits<float>::max();
            for(auto entity:game.getEntitys()) {
                auto point = entity->getIntersectionPoint(origin, mousePos);
                if(point) {
                    auto len0 = Vector2DistanceSqr(origin, entity->getPos());
                    if(len > len0) {
                        objectOnCoursor = rayCastData_t{entity, *point};
                        len = len0;
                    } 
                }
            }
        }

        BeginMode2D(camera);

            if(objectOnCoursor) {
                DrawLineEx(game.getPlayer()->getPos(), (*objectOnCoursor).pos, 1.0f, RED);
            } else {
                DrawLineEx(game.getPlayer()->getPos(), Vector2Add(Vector2Scale(Vector2Normalize(Vector2Subtract(mousePos, game.getPlayer()->getPos())), 1000.0f), game.getPlayer()->getPos()), 1.0f, GREEN);
            }

            for(const auto& entity:game.getEntitys()) {
                rlPushMatrix();
                    auto pos = entity->getPos();
                    rlTranslatef(pos.x, pos.y, 0);
                    entity->render();
                rlPopMatrix();
            }
            rlPushMatrix();
                rlTranslatef(game.getPlayer()->getPos().x, game.getPlayer()->getPos().y, 0);
                game.getPlayer()->render();
            rlPopMatrix();
        EndMode2D();

        
    }

    GameScreen::GameScreen() {
        camera.zoom = 1.5f;
        camera.rotation = 0.0f;
    }
    GameScreen::~GameScreen() {}
}