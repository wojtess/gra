#include "game.h"
#include <memory>
#include <math.h>
#include "rlImGui.h"
#include "imgui.h"

void Game::run() {
    InitWindow(1280, 720, "game");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    rlImGuiSetup(true);

    entitys.push_back(std::static_pointer_cast<PhysicsObject>(std::make_shared<Entity::Zombie>(Vector2{10.0f, 10.0f})));

    while(true) {
        {
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
                    ImGui::Text("player pos: x:%f, y:%f", player->getPos().x, player->getPos().y);
                    ImGui::Text("player vel: x:%f, y:%f", player->getVel().x, player->getVel().y);
                }
                ImGui::End();
            }
            


            //end drawning imgui
            rlImGuiEnd();
            //end drawning raylib
            EndDrawing();
        }
        

        if(this->player) {
            //tick world and shit
            {
                Vector2 velAdded = {0.0f, 0.0f};
                if(IsKeyDown(KEY_A)) {
                    velAdded.x -= 1;
                }
                if(IsKeyDown(KEY_D)) {
                    velAdded.x += 1;
                }
                if(IsKeyDown(KEY_W)) {
                    velAdded.y -= 1;
                }
                if(IsKeyDown(KEY_S)) {
                    velAdded.y += 1;
                }
                //normalize vector
                float len = sqrt(velAdded.x * velAdded.x + velAdded.y * velAdded.y);
                if(len != 0) {
                    Vector2 vel = this->player->getVel();
                    vel.x += velAdded.x / len;
                    vel.y += velAdded.y / len;
                    this->player->setVel(vel);
                }
                this->player->tick();
            }
        }

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