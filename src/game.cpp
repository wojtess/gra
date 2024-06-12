#include "game.h"
#include <memory>
#include <math.h>
#include "rlImGui.h"
#include "imgui.h"
#include "item.h"
#include "raymath.h"
#include <optional>
#include <cstdlib>

Game::Game(): resourceMap("../textures") {}

void Game::run() {
    InitWindow(1280, 720, "game");
    SetExitKey(KEY_NULL);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    rlImGuiSetup(true);

    while(!WindowShouldClose()) {
        //if player is null that means that world dont exist and we are on diffrent screen
        if(this->player) {
            if(this->player->getHp() <= 0) {
                this->player->stats.timeAlive = GetTime() - this->player->getStartTime();
                this->setScreen(std::make_unique<Screen::GameOver>(this->player->stats));
                this->player.reset();
                this->entitys.clear();
                continue;
            }
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
            //also to dont create another loop check how many zombies is there and and new if there is low count of zombies on map
            int zombieCount = 0;
            for(auto it = entitys.begin(); it != entitys.end();) {
                auto entity = *it;
                entity->tick(*this);
                if(std::dynamic_pointer_cast<Entity::Zombie>(entity)) {
                    zombieCount++;
                }

                {
                    auto item = std::dynamic_pointer_cast<Entity::DropedItem>(entity);
                    if(item) {
                        auto dist = Vector2Distance(this->player->getPos(), item->getPos());
                        if(player->isColliding(item)) {
                            auto& items = player->getItems();
                            bool removed = false;
                            for(int i = 0;i < items.size();i++) {
                                if(items[i] == nullptr) {
                                    items[i] = item->getItem();
                                    it = entitys.erase(it);
                                    removed = true;
                                    break;
                                }
                            }
                            if(removed) {
                                continue;
                            }
                        }
                    }
                }
                ++it;
            }

            if(zombieCount < ZOMBIE_COUNT) {
                for(int i = 0;i<ZOMBIE_COUNT-zombieCount;i++) {
                    auto zombie = std::make_shared<Entity::Zombie>(Vector2{(float)((rand() % 500 + 250) * ((rand() % 2) * 2 - 1)) + player->getPos().x, (float)((rand() % 500 + 250) * ((rand() % 2) * 2 - 1)) + player->getPos().y}, (rand() % (int)(ZOMBIE_MAX_SPEED + ZOMBIE_MIN_SPEED)) - ZOMBIE_MIN_SPEED);

                    if(zombie->getPos().x > MAX_X || zombie->getPos().x < MIN_X || zombie->getPos().y > MAX_Y || zombie->getPos().y < MIN_Y) {
                        i--;
                        continue;
                    }

                    bool kolizja = false;
                    for(const auto& e:entitys) {
                        if(zombie->isColliding(e)) {
                            //jest kolizja z entity
                            kolizja = true;
                            break;
                        }
                    }
                    if(kolizja) {
                        i--;
                    } else {
                        entitys.push_back(zombie);
                    }
                }
            }

        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
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

        auto item = player->getSelectedItem();
        if (item) {
            auto gun = std::dynamic_pointer_cast<Items::GunItem>(*item);
            if(gun) {
                std::shared_ptr<Entity::Zombie> zombie;
                if (objectOnCoursor) {
                    zombie = std::dynamic_pointer_cast<Entity::Zombie>(objectOnCoursor->object);   
                }
                //if firerate exist that means that gun is automatic
                if(gun->getFireRate()) {
                    if(gun->shoot(zombie)) {
                        player->stats.shotedBullets++;
                    }
                } else {
                    //when firerate dont exist that means that gun in not automatic
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if(gun->shoot(zombie)) {
                            player->stats.shotedBullets++;
                        }
                    }
                }
            }
        };

        
        }

        if(IsKeyPressed(KEY_E)) {
           auto item = player->getSelectedItem();
            if (item) {
                (*item)->use(player);
            }; 
        }

        for(auto& item:player->getItems()) {
            if(item) {
                auto action = item->getCurrentAction();
                if(action) {
                    if((*action).getRemainingTime() <= 0) {
                        (*action).callCallback();
                        item->clearAction();
                    }
                }
            }
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
            static bool debug = false;
            if(IsKeyReleased(KEY_F10)) {
                debug = !debug;
            }
            if(debug) {
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

std::shared_ptr<Building> createBuilding(float size, Vector2 pos, Color color) {     return std::make_shared<Building>(std::vector<Vector2>{Vector2{0.0f + pos.x, 0.0f + pos.y}, Vector2{(100.1f * size) + pos.x, 0.0f + pos.y}, Vector2{(100.f * size) + pos.x, (100.1f * size) + pos.y}, Vector2{0.0f + pos.x, (100.0f * size) + pos.y}}, color); }

void Game::newGame() {
    this->player = std::make_shared<Entity::Player>(Vector2{0.0f, 0.0f});

    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(1.0f, Vector2{50.0f, 50.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(1.5f, Vector2{200.0f, 60.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(1.0f, Vector2{30.0f, 300.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(0.5f, Vector2{-70.0f, 10.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(0.8f, Vector2{-90.0f, -200.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(1.2f, Vector2{0.1f, -500.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(2.0f, Vector2{100.0f,-400.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(3.0f, Vector2{20.0f, 600.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(1.3f, Vector2{-10.0f, 200.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(5.0f, Vector2{-40.0f, 700.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(1.5f, Vector2{-350.0f, -400.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(0.5f, Vector2{500.0f, 3.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(2.0f, Vector2{250.0f, 300.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(3.0f, Vector2{800.0f, 600.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(2.5f, Vector2{500.0f, -20.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(3.0f, Vector2{500.0f, 100.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(2.0f, Vector2{400.0f, -150.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(1.7f, Vector2{600.0f, -50.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(3.0f, Vector2{-500.0f, -100.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(2.0f, Vector2{-400.0f, 200.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(2.0f, Vector2{-600.0f, -600.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(3.0f, Vector2{-700.0f, 800.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(3.0f, Vector2{800.0f, -300.0f}, GRAY)));
    entitys.push_back(std::static_pointer_cast<PhysicsObject>(createBuilding(2.0f, Vector2{-400.0f, 300.0f}, GRAY)));
    
    srand( time( NULL ) );

    //dodawanie losowych itemow
    for(int i = 0;i <= ITEM_COUNT;i++) {
        std::shared_ptr<Items::GunItem> item;
        int r = rand() % 2;
        if(r == 1) {
            item = std::make_shared<Items::AkMachineGun>();
        } else {
            item = std::make_shared<Items::Pistol>();
        }
        auto nowyItemek = std::make_shared<Entity::DropedItem>(item, Vector2{(float)(rand() % (int)(MAX_X - MIN_X) + MIN_X), (float)(rand() % (int)(MAX_Y - MIN_Y) + MIN_Y)});
        bool kolizja = false;
        for(const auto& e:entitys) {
            if(nowyItemek->isColliding(e)) {
                //jest kolizja z entity
                kolizja = true;
                break;
            }
        }
        if(kolizja) {
            i--;
        } else {
            entitys.push_back(nowyItemek);
        }
    }
}

void Game::setScreen(std::unique_ptr<Screen::AbstractScreen> screen) {
    entitys.clear();
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

Renderer::ResourceMap& Game::getResourceMap() {
    return resourceMap;
}