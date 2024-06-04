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

    /*
        pausing system is unique and was created to dont create new varible in Game class.
        It was done more as exercise and to check if it can be done this way,
        there is absolutely no reason to do this this way.
    */

    void GameScreen::pause(Game& game) {
        paused = true;
        //save player from game
        this->player = game.getPlayer();
        //then set player in game to nullptr to stop physics
        game.setPlayer(nullptr);
    }

    void GameScreen::resume(Game& game) {
        paused = false;
        //set game player from saved player
        game.setPlayer(this->player);
        //dont reset this->player, beacuse it is used only when paused is false
        //It could create small memory leak, but probably screen in game will be changed when player is
        //no longer needed thefore calling destructor in this->player
    }

    void GameScreen::render(Game& game) {
        if(IsKeyReleased(KEY_ESCAPE)) {
            if(!paused) {
                pause(game);
            } else {
                resume(game);
            }
        }

        std::shared_ptr<Entity::Player> player;
        //if game is paused then game.getPlayer() will return nullptr sa we need to use saved pointer
        if(paused) {
            player = this->player;
        } else if(!game.getPlayer()) {
            //if player is nullptr and game is not paused that means that somthink is wrong
            printf("player is null\n");
            return;
        } else {
            //when player from game is not null we can use it
            player = game.getPlayer();
        }

        //update mouse position only when not paused
        if(!paused)
            savedMousePos = GetMousePosition();

        auto width = GetScreenWidth();
        auto height = GetScreenHeight();

        Hud::Button button([&game]() {
            game.setScreen(std::make_unique<Screen::HomeScreen>());
        }, "back", {0,0}, {200, 200}, 20);

        // button.render();

        camera.offset = Vector2{width / 2.0f, height / 2.0f};
        camera.target = player->getPos();


        auto mousePos = Vector2Add(player->getPos(), Vector2Scale(Vector2Subtract(savedMousePos, camera.offset), 1.0f/camera.zoom));
        struct rayCastData_t {
            std::shared_ptr<PhysicsObject> object;
            Vector2 pos;
        };
        std::optional<rayCastData_t> objectOnCoursor;
        {
            auto origin = player->getPos();

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
                DrawLineEx(player->getPos(), (*objectOnCoursor).pos, 1.0f, RED);
            } else {
                DrawLineEx(player->getPos(), Vector2Add(Vector2Scale(Vector2Normalize(Vector2Subtract(mousePos, player->getPos())), 1000.0f), player->getPos()), 1.0f, GREEN);
            }

            for(const auto& entity:game.getEntitys()) {
                rlPushMatrix();
                    auto pos = entity->getPos();
                    rlTranslatef(pos.x, pos.y, 0);
                    entity->render();
                rlPopMatrix();
            }
            rlPushMatrix();
                rlTranslatef(player->getPos().x, player->getPos().y, 0);
                player->render();
            rlPopMatrix();
        EndMode2D();

        //render hud
        {
            Vector2 size{80.0f, 80.0f};
            float y_offset = 15.0f;
            DrawRectangleLinesEx(Rectangle{(width - (size.x * STACK_SIZE)) / 2.0f, height - size.y - y_offset, size.x * STACK_SIZE, size.y}, 1.0f, BLACK);
            for (int i = 1; i < STACK_SIZE; i++) {
                DrawLineEx(Vector2{((width - (size.x * STACK_SIZE)) / 2.0f) + size.x * i, height - size.y - y_offset}, Vector2{((width - (size.x * STACK_SIZE)) / 2.0f) + size.x * i,  height - size.y - y_offset + size.y}, 1.0f, BLACK);
            }
            {
                int selected = player->getSelctedItemIndex();
                DrawRectangleLinesEx(Rectangle{((width - (size.x * STACK_SIZE)) / 2.0f) + size.x * selected, height - size.y - y_offset, size.x,  size.y}, 2.0f, RED);
            }
            
            for (int i = 0; i < player->getItems().size(); i++) {
                auto item = player->getItems()[i];
                rlPushMatrix();
                    rlTranslatef(((width - (size.x * STACK_SIZE)) / 2.0f) + size.x * i + size.x / 2, height - size.y - y_offset + size.y / 2, 0.0f);
                    item->render();
                rlPopMatrix();
            }
        }

        if(paused) {
            DrawRectangle(0,0, width, height, Color{0x0, 0x0, 0x0, 150});
            DrawText("paused", width / 2, height / 2, 20, RED);
        }
    }

    GameScreen::GameScreen(): paused(false) {
        camera.zoom = 1.5f;
        camera.rotation = 0.0f;
    }
    GameScreen::~GameScreen() {}
}