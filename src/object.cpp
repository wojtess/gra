#include "object.h"
#include "utils.h"
#include "math.h"
#include "raymath.h"
#include <algorithm>
#include <limits>
#include "game.h"
#include "constants.h"

void Entity::Zombie::setHp(int nowe) {
    hp = nowe;
}

int Entity::Zombie::getHp() {
    return hp;
}

namespace Shapes {
    
    Circle::Circle(float radius): radius(radius) {

    }

    std::optional<Vector2> Circle::isColliding(const std::shared_ptr<AbstractShape>& other) {
        auto circle = std::dynamic_pointer_cast<Circle>(other);
        if(circle) {
            if(Vector2Distance(pos, circle->pos) <= radius + circle->radius) {
                return Vector2Subtract(pos, circle->pos);
            }
            return {};
        } else {
            auto shape = std::dynamic_pointer_cast<Shape>(other);
            if (shape) {
                //separating axis theorem tells us that we need to draw a line beetwen two objects to tell if there are overlaping or not
                //to know where to find these lines we create perperdicular vector to each edge and project shape to these vectors
                //easy explonation to quicky code it: http://programmerart.weebly.com/separating-axis-theorem.html
                //other material I found: https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/previousinformation/physics4collisiondetection/2017%20Tutorial%204%20-%20Collision%20Detection.pdf
                //to Separating Axis Theorem to work, vertices need to be in right order, we need lines that are sides not diagonals lines

                std::vector<Vector2> vertices = shape->getVertices();
                
                //circle have inf "sides" so we need to get one that perpedicular vector is shortest to shape
                Vector2 closestPoint = vertices[0];
                float minDist = Vector2Length(Vector2Subtract(pos, closestPoint));
                //search for shortest vector from all vertices
                for (const auto& vertex:vertices) {
                    float dist = Vector2Length(Vector2Subtract(pos, vertex));
                    if (dist < minDist) {
                        minDist = dist;
                        closestPoint = vertex;
                    }
                }
                //then normalie it, we dont need length, we only need direction
                Vector2 direction = Vector2Normalize(Vector2Subtract(pos, closestPoint));

                //then we need to find all perpedicular vectors for our shape
                //we only need direction so all are normalized
                std::vector<Vector2> axes;
                {
                    //we need to get edge that comes from last element to first element, beacuse loop dosent include it
                    Vector2 edge = Vector2Subtract(vertices[vertices.size() - 1], vertices[0]);
                    axes.push_back(Vector2Normalize(Vector2{-edge.y, edge.x}));
                }
                //then loop over every edge and add its perpedicular vector to list
                for (size_t i = 1; i < vertices.size(); i++) {
                    Vector2 edge = Vector2Subtract(vertices[i - 1], vertices[i]);
                    axes.push_back(Vector2Normalize(Vector2{-edge.y, edge.x}));
                }
                axes.push_back(direction);

                float minOverlap = std::numeric_limits<float>::infinity();
                Vector2 collisionEdge;

                //last step is to project all vertices to axes and see if they overlap
                for (const auto& axis : axes) {
                    float minA = std::numeric_limits<float>::infinity();
                    float maxA = -std::numeric_limits<float>::infinity();
                    //we check for every vertex and then pick one that are cover largests area on our 1D line
                    for (const auto& vertex : vertices) {
                        float projection = Vector2DotProduct(vertex, axis);
                        minA = std::min(minA, projection);
                        maxA = std::max(maxA, projection);
                    }

                    //same for circle, circle is same in all direction so it is simple
                    float projectionCircle = Vector2DotProduct(pos, axis);
                    float minB = projectionCircle - radius;
                    float maxB = projectionCircle + radius;

                    //check if they overlap
                    if (maxA < minB || maxB < minA) {
                        //dont overlap, no collision
                        return {};
                    } else {
                        //overlap, save distace and axis, lower distance is edge that we are colliding with
                        float overlap = std::min(maxA, maxB) - std::max(minA, minB);
                        if (overlap < minOverlap) {
                            minOverlap = overlap;
                            collisionEdge = axis;
                        }
                    }
                }
                return collisionEdge;
            }
            //should not happend
            return {};
        }
    }

    std::vector<Vector2> Circle::getVertices() {
        //How I am suposed to return all infinitly many vertices????
        return {};
    }

    void Circle::setPos(Vector2 pos) {
        this->pos = pos;
    }

    Vector2 Circle::getPos() const {
        return pos;
    }

    float Circle::getRadius() const {
        return radius;
    }

    Shape::Shape(std::vector<Vector2> vertices) {
        Vector2 centroid = {0, 0};
        for (const auto& vertex : vertices) {
            centroid = Vector2Add(centroid, vertex);
        }
        centroid = Vector2Scale(centroid, (1.0f / vertices.size()));

        std::sort(vertices.begin(), vertices.end(), [centroid](const Vector2& a, const Vector2& b) {
            auto aAngle = Vector2LineAngle(Vector2{0,0}, Vector2Subtract(a, centroid));
            auto bAngle = Vector2LineAngle(Vector2{0,0}, Vector2Subtract(b, centroid));
            return aAngle > bAngle;
        });

        this->vertices = vertices;
    }

    std::optional<Vector2> Shape::isColliding(const std::shared_ptr<AbstractShape>& other) {
        //for now unused, all collisions is checked from player "perpsecive"
        return {};
    }

    std::vector<Vector2> Shape::getVertices() {
        //clone vertices and then add offset to them
        //it is best peroforming solution, but years of hardware progress made it posible to write slow code
        std::vector<Vector2> verticesClone;
        for(auto v:vertices) {
            v.x += offset.x;
            v.y += offset.y;
            verticesClone.push_back(v);
        }
        return verticesClone;
    }

    void Shape::setPos(Vector2 pos) {
        this->offset = pos;
    }

}

PhysicsObject::PhysicsObject(float dumpingFactor): dumpingFactor(dumpingFactor), vel(Vector2{0.0f, 0.0f}), accel(Vector2{0.0f, 0.0f}), physicsOn(true) {
    pos = { 0 };
}

PhysicsObject::PhysicsObject(): dumpingFactor(10.0f), vel(Vector2{0.0f, 0.0f}), accel(Vector2{0.0f, 0.0f}), physicsOn(true) {
    pos = { 0 };
}

void PhysicsObject::setVel(Vector2 vel) {
    this->vel = vel;
}

Vector2 PhysicsObject::getVel() const {
    return vel;
}

void PhysicsObject::setAccel(Vector2 accel) {
    this->accel = accel;
}

Vector2 PhysicsObject::getAccel() const {
    return this->accel;
}

void PhysicsObject::applyFrixion() {
    //after solving this equations: dv/dt = -dumpingFactor * v
    //this is solution: v = v_0 * e^(dumpingFactor * t)
    //for discrete opertions: v(t + dt) = v(t) * e^(dumpingFactor * dt)
    vel.x *= exp(-dumpingFactor * GetFrameTime());
    vel.y *= exp(-dumpingFactor * GetFrameTime());
}

void PhysicsObject::tick(Game& game) {
    vel.x += accel.x * GetFrameTime();
    vel.y += accel.y * GetFrameTime();

    applyFrixion();

    for(auto& shape:shapes) {
        Vector2 checkingPos = {pos.x + vel.x * GetFrameTime(), pos.y + vel.y * GetFrameTime()};
        shape->setPos(checkingPos);
        for(auto& entity:game.getEntitys()) {
            if(entity.get() == this) {
                //dont check collisions with itself
                continue;
            }
            if(!entity->physicsOn) {
                //some object can have physics disabled, but still have boundix boxes(items)
                continue;
            }

            for(auto& otherShape:entity->shapes) {
                if(auto vec = shape->isColliding(otherShape)) {
                    Vector2 normal = Vector2Normalize(*vec);
                    float dot = Vector2DotProduct(vel, normal);
                    if(dot < 0) {
                        Vector2 projection = {dot * normal.x, dot * normal.y};
                        vel = Vector2Subtract(vel, projection);
                    }
                }
            }
        }
        if((PhysicsObject*)game.getPlayer().get() != this) {
            for(auto& otherShape:game.getPlayer()->shapes) {
                if(auto vec = shape->isColliding(otherShape)) {
                    Vector2 normal = Vector2Normalize(*vec);
                    float dot = Vector2DotProduct(vel, normal);
                    
                    vel = Vector2{0, 0};
                    if(dot < 0) {
                        Vector2 projection = {dot * normal.x, dot * normal.y};
                        vel = Vector2Subtract(vel, projection);
                    }
                }
            }
        }
    }

    pos.x += vel.x * GetFrameTime();
    pos.y += vel.y * GetFrameTime();

    for(auto& shape : shapes) {
        shape->setPos(pos);
    }
}

std::optional<Vector2> PhysicsObject::getIntersectionPoint(Vector2 origin, Vector2 end) {
    float len = std::numeric_limits<float>::max();
    Vector2 out;
    bool found = false;

    for(const auto& shape:shapes) {
        //check for Shapes::Shape
        Shapes::Shape* shape0 = dynamic_cast<Shapes::Shape*>(shape.get());
        if(shape0) {
            shape->setPos(pos);
            auto vertices = shape->getVertices();
            {
                if(auto out0 = RayCastUtils::intersectionLine(origin, Vector2Normalize(Vector2Subtract(end, origin)), vertices[0], vertices[vertices.size() - 1])) {
                    out = *out0;
                    len = Vector2LengthSqr(Vector2Subtract(origin, *out0));
                    //withgout check if(!found), beacuse it is faster to just move 1 to found than checking and then moving(1 instruction vs 3 instrcution at least)
                    found = true;
                }
            }
            for(size_t i = 1; i < vertices.size(); i++) {
                if(auto out0 = RayCastUtils::intersectionLine(origin, Vector2Normalize(Vector2Subtract(end, origin)), vertices[i - 1], vertices[i])) {
                    auto len0 = Vector2LengthSqr(Vector2Subtract(origin, *out0));
                    if(len > len0) {
                        out = *out0;
                        len = len0;
                        found = true;
                    }
                }
                
            }
        } else {
            //check for circle
            Shapes::Circle* circle = dynamic_cast<Shapes::Circle*>(shape.get());
            if(circle) {
                if(auto out0 = RayCastUtils::intersectionCircle(origin, Vector2Normalize(Vector2Subtract(end, origin)), circle->getPos(), circle->getRadius())) {
                    auto len0 = Vector2LengthSqr(Vector2Subtract(origin, *out0));
                    if(len > len0) {
                        out = *out0;
                        len = len0;
                        found = true;
                    }
                }
            }
        }
    }
    if(found)
        return out;
    return {};
}

bool PhysicsObject::isColliding(const PhysicsObject& other) {
    //check for collisions for every shape in current object and in "other" object
    //but moslt it is only one shape per object
    for(const auto& shape : shapes) {
        for(const auto& otherShape:other.shapes) {
            if(shape->isColliding(otherShape)) {
                return true;
            }
        }
    }
    return false;
}

bool PhysicsObject::isColliding(const PhysicsObject* other) {
    //same as PhysicsObject::isColliding(const PhysicsObject&), but for pointer
    for(const auto& shape : shapes) {
        for(auto& otherShape:other->shapes) {
            if(shape->isColliding(otherShape)) {
                return true;
            }
        }
    }
    return false;
}

bool PhysicsObject::isColliding(const std::shared_ptr<PhysicsObject> other) {
    //same as PhysicsObject::isColliding(const PhysicsObject&), but for pointer
    for(const auto& shape : shapes) {
        for(auto& otherShape:other->shapes) {
            if(shape->isColliding(otherShape)) {
                return true;
            }
        }
    }
    return false;
}

namespace Entity {
    Zombie::Zombie(Vector2 pos) {
        this->pos = pos;
        shapes.push_back(std::make_shared<Shapes::Circle>(10.0f));
        for(auto& shape:shapes) {
            shape->setPos(pos);
        }
        this->hp = ZOMBIE_HP;
    }

    void Zombie::render(Renderer::ResourceMap& resourceMap) {
        DrawCircleV(Vector2{0.f, 0.f}, 10.0f, Color{0, 0, 0, 10});
        auto tex = resourceMap.getTexture("zombie.png");
        DrawTexturePro(tex, Rectangle{0,0,(float)tex.width,(float)tex.height}, Rectangle{0,0,40,40}, Vector2{20, 20}, 0.0f, WHITE);
    }

    void Zombie::tick(Game& game) {
        if (hp <= 0) {
            auto& entitys = game.getEntitys();
            auto iterator = entitys.begin();
            while(iterator != entitys.end()) {
                if((*iterator).get() == (PhysicsObject*)(this)) {
                    iterator = entitys.erase(iterator);
                } else {
                    iterator++;
                }
            }
            entitys.push_back(std::static_pointer_cast<PhysicsObject>(std::make_shared<Entity::DropedItem>(std::make_unique<Items::MedkitItem>(), getPos())));
            return;
        };


        auto player = game.getPlayer();
        auto playerPos = player->getPos();

        if(Vector2Distance(playerPos, getPos()) < ZOMBIE_ATTACK_DISTNACE) {
            player->setHp(player->getHp() - ZOMBIE_DAMAGE * GetFrameTime());
        }

        if(Vector2Distance(playerPos, getPos()) < ZOMBIE_SIGHT) {
            //directtion where is player
            auto dir = Vector2Normalize(Vector2Subtract(playerPos, getPos()));
            //zombie are stupid so they just go where they see player
            for(auto e:game.getEntitys()) {
                //check if object(e varible) is this zombie
                if((void*)e.get() != (void*)this) {
                    //check if e is zombie or dropped item, zobie can see throught other zombies as well droppeditems
                    if(std::dynamic_pointer_cast<Zombie>(e) || std::dynamic_pointer_cast<DropedItem>(e)) {
                        continue;
                    }
                    //check if zombie(this) is looking at object
                    if(auto interscetion = e->getIntersectionPoint(getPos(), Vector2Add(getPos(), Vector2Scale(dir, 100.0f)))) {
                        //if zombie is looking at e check if object is befoer player
                        //this means that player is hidden behind object
                        if(Vector2DistanceSqr(getPos(), *interscetion) < Vector2DistanceSqr(getPos(), playerPos)) {
                            goto setAccelToZero;
                        }
                    }
                }
            }
            setAccel(Vector2Scale(dir, 800.0f));
        } else {
            setAccelToZero:
            setAccel(Vector2{0.0f, 0.0f});
        }
        PhysicsObject::tick(game);
    }

    Player::Player(Vector2 pos): PhysicsObject(8.f), selectedItem(0) {
        this->pos = pos;
        shapes.push_back(std::make_shared<Shapes::Circle>(10.0f));
        for(auto& shape:shapes) {
            shape->setPos(pos);
        }

        hp = PLAYER_HP;
    }

    void Player::render(Renderer::ResourceMap& resourceMap) {
        DrawCircleV(Vector2{0.f, 0.f}, 10.0f, Color{0, 0, 0, 40});
        auto tex = resourceMap.getTexture("guy.png");
        DrawTexturePro(tex, Rectangle{0,0,(float)tex.width,(float)tex.height}, Rectangle{0,0,20,20}, Vector2{10, 10}, 0.0f, WHITE);
    }

    int Player::getSelctedItemIndex() {
        return selectedItem;
    }

    void Player::setHp(float nowe) {
        hp = nowe;
    }

    float Player::getHp() {
        return hp;
    }

    std::optional<std::shared_ptr<Items::AbstractItem>> Player::getSelectedItem() {
        if(selectedItem < 0 || selectedItem >= items.size() || items[selectedItem] == nullptr) {
            return {};
        }
        return items[selectedItem];
    }
    
    std::array<std::shared_ptr<Items::AbstractItem>, STACK_SIZE>& Player::getItems() {
        return items;
    }

    void Player::setLookingDirection(Vector2 v) {
        lookingDirection = v;
    }

    Vector2 Player::getLookingDirection() const {
        return lookingDirection;
    }

    DropedItem::DropedItem(std::shared_ptr<Items::AbstractItem> item, Vector2 pos): item(item) {
        this->pos = pos;
        shapes.push_back(std::make_shared<Shapes::Circle>(item->getPickupDistance()));
        physicsOn = false;
        for(auto& shape:shapes) {
            shape->setPos(pos);
        }
    }

    void DropedItem::render(Renderer::ResourceMap& resourceMap) {
        item->render(resourceMap);
    }

    std::shared_ptr<Items::AbstractItem>& DropedItem::getItem() {
        return item;
    }

    void Player::incraseSelectedItemIndex() {
        selectedItem++;
        if(selectedItem >= items.size()) {
            selectedItem = 0;
        }
    }

    void Player::decraseSelectedItemIndex() {
        selectedItem--;
        if(selectedItem < 0) {
            //wont work if items.size() is equal to zero
            selectedItem = items.size() - 1;
        }
    }
}

Building::Building(std::vector<Vector2> vertices, Color color): color(color) {
    shapes.push_back(std::make_shared<Shapes::Shape>(vertices));
}

Building::Building(std::vector<Vector2> vertices, Color color, Vector2 pos): color(color) {
    auto shape = std::make_shared<Shapes::Shape>(vertices);
    shape->setPos(pos);
    shapes.push_back(std::move(shape));
}

void Building::render(Renderer::ResourceMap& resourceMap) {
    auto vertices = shapes[0]->getVertices();
    
    for(int i = 2;i < vertices.size(); i++) {
        Vector2 v2 = vertices[0];
        Vector2 v1 = vertices[i - 1];
        Vector2 v3 = vertices[i];

        if((v2.x - v1.x) * (v2.y + v1.y) + (v2.x - v3.x) * (v2.y + v3.y) > 0) {
            std::swap(v2, v3);
        }
        DrawTriangle(v1, v2, v3, color);
    }
}

namespace Hud {
    Button::Button(std::function<void()> onClick, std::string label, Vector2 pos, Vector2 size, int fontSize, Renderer::Theme theme): onClick(onClick), label(label), pos(pos), size(size), theme(theme), fontSize(fontSize) {
        //center label
        int defaultFontSize = 10;
        if (fontSize < defaultFontSize) fontSize = defaultFontSize;
        int spacing = fontSize/defaultFontSize;
        auto textSize = MeasureTextEx(GetFontDefault(), label.c_str(), (float)fontSize, (float)spacing);
        textPos.x = (size.x - textSize.x) / 2 + pos.x;
        textPos.y = (size.y - textSize.y) / 2 + pos.y;
    }
    void Button::render(Renderer::ResourceMap& resourceMap) {
        bool inside = isInside(GetMousePosition(), pos, size);
        if(inside) {
            DrawRectangleRec({pos.x, pos.y, size.x, size.y}, theme.getHover());
        } else {
            DrawRectangleRec({pos.x, pos.y, size.x, size.y}, theme.getBg());
        }
        DrawText(label.c_str(), textPos.x, textPos.y, fontSize, theme.getTextColor());
            
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            auto mousePos = GetMousePosition();
            if(inside) {
                onClick();
            }
        }
    }

    Label::Label(std::string label, Vector2 pos, Color color, int fontSize): label(label), color(color), fontSize(fontSize) {
        //center label
        int defaultFontSize = 10;
        if (fontSize < defaultFontSize) fontSize = defaultFontSize;
        int spacing = fontSize/defaultFontSize;
        auto textSize = MeasureTextEx(GetFontDefault(), label.c_str(), (float)fontSize, (float)spacing);
        Vector2 textPos;
        textPos.x = (textSize.x) / -2 + pos.x;
        textPos.y = (textSize.y) / -2 + pos.y;
        this->pos = textPos;
    }
    void Label::render(Renderer::ResourceMap& resourceMap) {
        DrawText(label.c_str(), pos.x, pos.y, fontSize, color);
    }
};