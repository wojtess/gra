#include "object.h"
#include "utils.h"
#include "math.h"
#include "raymath.h"

namespace Shape {
    
    Circle::Circle(float radius): radius(radius) {

    }

    bool Circle::isColliding(const std::unique_ptr<AbstractShape>& other) {
        Circle* circle = (Circle*)other.get();
        if(circle) {
            if(Vector2Distance(pos, circle->pos) <= radius + circle->radius) {
                return true;
            }
        }
        return false;
    }

    std::vector<Vector2> Circle::getVertices(std::unique_ptr<AbstractShape>& other) {
        return {};
    }

    void Circle::setPos(Vector2 pos) {
        this->pos = pos;
    }

    bool Shape::isColliding(const std::unique_ptr<AbstractShape>& other) {

        return false;
    }

    std::vector<Vector2> Shape::getVertices(std::unique_ptr<AbstractShape>& other) {
        return {};
    }

}

PhysicsObject::PhysicsObject(float dumpingFactor): dumpingFactor(dumpingFactor), vel(Vector2{0.0f, 0.0f}) {

}

PhysicsObject::PhysicsObject(): dumpingFactor(10.0f), vel(Vector2{0.0f, 0.0f}) {

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
    vel.x *= exp(-dumpingFactor * GetFrameTime());
    vel.y *= exp(-dumpingFactor * GetFrameTime());
}

void PhysicsObject::tick() {
    vel.x += accel.x * GetFrameTime();
    vel.y += accel.y * GetFrameTime();

    applyFrixion();

    pos.x += vel.x * GetFrameTime();
    pos.y += vel.y * GetFrameTime();

    for(auto& shape : shapes) {
        shape->setPos(pos);
    }
}

bool PhysicsObject::isColliding(const PhysicsObject& other) {
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
        shapes.push_back(std::make_unique<Shape::Circle>(10.0f));
    }

    void Zombie::render() {
        DrawCircle(0, 0, 10, RED);
    }

    Player::Player(Vector2 pos): PhysicsObject(8.f) {
        this->pos = pos;
        shapes.push_back(std::make_unique<Shape::Circle>(10.0f));
    }

    void Player::render() {
        DrawCircle(0, 0, 10, BLUE);
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
    void Button::render() {
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
    void Label::render() {
        DrawText(label.c_str(), pos.x, pos.y, fontSize, color);
    }
};