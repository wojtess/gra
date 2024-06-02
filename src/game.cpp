#include "game.h"
#include <memory>
#include <math.h>
#include "rlImGui.h"
#include "imgui.h"

void Game::run() {
    InitWindow(1280, 720, "game");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    rlImGuiSetup(true);

    entitys.push_back(std::static_pointer_cast<PhysicsObject>(std::make_shared<Entity::Zombie>(Vector2{100.0f, 100.0f})));
    // entitys.push_back(std::static_pointer_cast<PhysicsObject>(std::make_shared<Building>(std::vector<Vector2>{Vector2{20.0f, 20.0f}, Vector2{200.0f, .0f}, Vector2{.0f, 200.0f}, Vector2{200.0f, 200.0f}}, RED)));

    while(true) {
        if(this->player) {
            //tick world and shit
            {
                Vector2 accel = {0.0f, 0.0f};
                if(IsKeyDown(KEY_A)) {
                    accel.x -= 1;
                }
                if(IsKeyDown(KEY_D)) {
                    accel.x += 1;
                }
                if(IsKeyDown(KEY_W)) {
                    accel.y -= 1;
                }
                if(IsKeyDown(KEY_S)) {
                    accel.y += 1;
                }
                //normalize vector
                float len = sqrt(accel.x * accel.x + accel.y * accel.y);
                Vector2 accel1 = {0};
                float accelMultplayer = 1000.0f;
                if(len != 0) {
                    accel1.x = (accel.x / len) * accelMultplayer;
                    accel1.y = (accel.y / len) * accelMultplayer;
                }
                this->player->setAccel(accel1);
                this->player->tick(entitys);
            }

            for(const auto& entity:entitys) {
                entity->tick(entitys);
            }
        }


        //begin drawning raylib
        BeginDrawing();
        //clear background is needed for using some debug/imgui rendering when screen is null, every screen is probably have some other bg
        ClearBackground(RAYWHITE);
        //begin drawning imgui
        rlImGuiBegin();
        
        //we dont want to have seg fault so we need to check if screen is nullptr
        if(this->screen)
            this->screen->render(*this);

        {
            ImGui::Begin("debug");
            if(player) {
                int colliding = 0;
                for(const auto& entity:entitys) {
                    if(player->isColliding(entity.get()))
                        colliding++;
                }
                ImGui::Text("player pos: x:%f, y:%f", player->getPos().x, player->getPos().y);
                ImGui::Text("player vel: x:%f, y:%f", player->getVel().x, player->getVel().y);
                ImGui::Text("player accel: x:%f, y:%f", player->getAccel().x, player->getAccel().y);
                ImGui::Text("player colliding: %d", colliding);
            }
            ImGui::End();
        }
        
        //end drawning imgui
        rlImGuiEnd();
        //end drawning raylib
        EndDrawing();
    }


    //this isnt necesary beacuse when exiting program OS should clean everything
    rlImGuiShutdown();
    CloseWindow();
}

void Game::newGame() {
    this->player = std::make_shared<Entity::Player>(Vector2{0.0f, 0.0f});
}

void Game::setScreen(std::unique_ptr<Screen::AbstractScreen> screen) {
    this->screen = std::move(screen);
}

std::vector<std::shared_ptr<PhysicsObject>> Game::getEntitys() const {
    return entitys;
}

std::shared_ptr<Entity::Player> Game::getPlayer() const {
    return player;
}