#include "game.h"
#include <memory>
#include <math.h>
#include "rlImGui.h"
#include "imgui.h"
#include "item.h"
#include "raymath.h"
#include <optional>

void Game::run() {
    InitWindow(1280, 720, "game");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    rlImGuiSetup(true);

    entitys.push_back(std::static_pointer_cast<PhysicsObject>(std::make_shared<Entity::Zombie>(Vector2{-200.0f, -100.0f})));

    entitys.push_back(std::static_pointer_cast<PhysicsObject>(std::make_shared<Building>(std::vector<Vector2>{Vector2{20.0f, 20.0f}, Vector2{200.0f, .0f}, Vector2{.0f, 200.0f}, Vector2{200.0f, 200.0f}}, RED)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(std::make_shared<Entity::DropedItem>(std::make_unique<Items::GunItem>(), Vector2{-100.0f, -100.0f})));

    while(true) {
        //if player is null that means that world dont exist and we are on diffrent screen
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
                this->player->tick(*this);
            }
            {
                if(IsKeyDown(KEY_Q)) {
                    int selectedItem = this->player->getSelctedItemIndex();
                    auto& items = this->player->getItems();

                    if(items[selectedItem] != nullptr) {
                        auto item = items[selectedItem];

                        auto lookingDir = this->player->getLookingDirection();
                        auto playerPos = this->player->getPos();
                        //multiplaying pickup distance by 2.5f is cheap trick to make it work,
                        //item should be dropped in playerPos + lookingDir * (pickupDistance + playerSize),
                        //where playerSize is radius in case if player is circle or something else like rectangle size in that direction
                        auto droppedItem = std::make_shared<Entity::DropedItem>(item, Vector2{playerPos.x + lookingDir.x * (item->getPickupDistance() * 2.5f), playerPos.y + lookingDir.y * (item->getPickupDistance() * 2.5f)});
                        droppedItem->setVel(Vector2Scale(lookingDir, 1000.0f));
                        //check if droppedItem collide with anythink, if it collide it means that it cant be dropped.
                        for(auto e:entitys) {
                            if(droppedItem->isColliding(e)) {
                                //dont add droppedItem to entitys
                                //or remove item from invenotry
                                goto skipDropping;
                            }
                        }

                        items[selectedItem] = nullptr;
                        entitys.push_back(std::static_pointer_cast<PhysicsObject>(droppedItem));
                    }
                }
                skipDropping:

                if(IsKeyDown(KEY_R)) {
                    auto itemek = player->getSelectedItem();
                    if (itemek) {
                        (*itemek)->use(player);
                    };
                }

                if(GetMouseWheelMove() > 0.1f) {
                    player->decraseSelectedItemIndex();
                } else if(GetMouseWheelMove() < -0.1f) {
                    player->incraseSelectedItemIndex();
                }
            }

            //tick entity and remove ones that are picked from the ground(items only);
            for(auto it = entitys.begin(); it != entitys.end();) {
                auto entity = *it;
                entity->tick(*this);
                {
                    auto item = std::dynamic_pointer_cast<Entity::DropedItem>(entity);
                    if(item) {
                        auto dist = Vector2Distance(this->player->getPos(), item->getPos());
                        if(player->isColliding(item)) {
                            auto& items = player->getItems();
                            for(int i = 0;i < items.size();i++) {
                                if(items[i] == nullptr) {
                                    items[i] = item->getItem();
                                    it = entitys.erase(it);
                                    //when we find item, exit loop and tell next loop that we need to continue,
                                    //we need to continue in outer loop(one that iterates over entitys)
                                    //beacuse we dont want to execute last instruction in this loop(it++;)
                                    goto removed;
                                }
                            }
                            //if we are here then skip `continue;`,
                            //`inc` label could be after `continue;`, but C dont allow this for some reason
                            goto inc;
                            removed:
                            continue;
                        }
                    }
                }
                inc:
                it++;
            }

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        struct rayCastData_t {
            std::shared_ptr<PhysicsObject> object;
            Vector2 pos;
        };
        std::optional<rayCastData_t> objectOnCoursor;
        {
            auto origin = player->getPos();

            float len = std::numeric_limits<float>::max();
            for(auto entity:getEntitys()) {
                //multply locking direction by 100.0f to be sure that our ray is idk words for this
                auto point = entity->getIntersectionPoint(origin, Vector2Add(Vector2Scale(player->getLookingDirection(), 100.0f), origin));
                if(point) {
                    auto len0 = Vector2DistanceSqr(origin, entity->getPos());
                    if(len > len0) {
                        objectOnCoursor = rayCastData_t{entity, *point};
                        len = len0;
                    }
                }
            }
        }


        if (objectOnCoursor) {
            auto zombieObject = std::dynamic_pointer_cast<Entity::Zombie>(objectOnCoursor->object);
            if (zombieObject) {
                auto item = player->getSelectedItem();
                if (item) {
                    auto gun = std::dynamic_pointer_cast<Items::GunItem>(*item);
                    if(gun) {
                        gun->shoot(zombieObject);
                    }
                };
            };
        } else {
            auto item = player->getSelectedItem();
                if (item) {
                    auto gun = std::dynamic_pointer_cast<Items::GunItem>(*item);
                    if(gun) {
                        auto zombie = std::shared_ptr<Entity::Zombie>();
                        gun->shoot(zombie);
                    }
                };
        };
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
                int items = 0;
                for(auto& item:player->getItems()) {
                    if(item) {
                        items++;
                    }
                }
                ImGui::Text("player items: %d", items);
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

std::vector<std::shared_ptr<PhysicsObject>>& Game::getEntitys() {
    return entitys;
}

std::shared_ptr<Entity::Player> Game::getPlayer() {
    return player;
}

void Game::setPlayer(std::shared_ptr<Entity::Player> player) {
    this->player = player;
}