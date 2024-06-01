#include "object.h"
#include "utils.h"
#include "math.h"
#include "raymath.h"
#include <algorithm>
#include <limits>

namespace Shapes {
    
    Circle::Circle(float radius): radius(radius) {

    }

    bool Circle::isColliding(const std::unique_ptr<AbstractShape>& other) {
        Circle* circle = dynamic_cast<Circle*>(other.get());
        if(circle) {
            printf("circle\n");
            if(Vector2Distance(pos, circle->pos) <= radius + circle->radius) {
                return true;
            }
            return false;
        } else {
            Shape* shape = dynamic_cast<Shape*>(other.get());
            if (shape) {
                std::vector<Vector2> vertices = shape->getVertices();
                Vector2 closestPoint = vertices[0];
                float minDist = Vector2Length(Vector2Subtract(pos, closestPoint));

                for (const auto& vertex : vertices) {
                    float dist = Vector2Length(Vector2Subtract(pos, vertex));
                    if (dist < minDist) {
                        minDist = dist;
                        closestPoint = vertex;
                    }
                }

                Vector2 direction = Vector2Normalize(Vector2Subtract(pos, closestPoint));

                std::vector<Vector2> axes;
                {
                    Vector2 edge = Vector2Subtract(vertices[vertices.size() - 1], vertices[0]);
                    axes.push_back(Vector2Normalize(Vector2{-edge.y, edge.x}));
                }
                for (size_t i = 1; i < vertices.size(); i++) {
                    Vector2 edge = Vector2Subtract(vertices[i - 1], vertices[i]);
                    axes.push_back(Vector2Normalize(Vector2{-edge.y, edge.x}));
                }
                axes.push_back(direction);

                for (const auto& axis : axes) {
                    float minA = std::numeric_limits<float>::infinity();
                    float maxA = -std::numeric_limits<float>::infinity();
                    for (const auto& vertex : vertices) {
                        float projection = Vector2DotProduct(vertex, axis);
                        minA = std::min(minA, projection);
                        maxA = std::max(maxA, projection);
                    }

                    float projectionCircle = Vector2DotProduct(pos, axis);
                    float minB = projectionCircle - radius;
                    float maxB = projectionCircle + radius;

                    if (maxA < minB || maxB < minA) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }
    }

    std::vector<Vector2> Circle::getVertices() {
        //How I am suposed to return all infinitly many vertices????
        return {};
    }

    void Circle::setPos(Vector2 pos) {
        this->pos = pos;
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

    bool Shape::isColliding(const std::unique_ptr<AbstractShape>& other) {
        return false;
    }

    std::vector<Vector2> Shape::getVertices() {
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
        shapes.push_back(std::make_unique<Shapes::Circle>(10.0f));
    }

    void Zombie::render() {
        DrawCircle(0, 0, 10, RED);
    }

    Player::Player(Vector2 pos): PhysicsObject(8.f) {
        this->pos = pos;
        shapes.push_back(std::make_unique<Shapes::Circle>(10.0f));
    }

    void Player::render() {
        DrawCircle(0, 0, 10, BLUE);
    }
}

Building::Building(std::vector<Vector2> vertices, Color color): color(color) {
    shapes.push_back(std::make_unique<Shapes::Shape>(vertices));
    accel = { 0 };
    vel = { 0 };
    pos = { 0 };
}

void Building::render() {
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

    // {
    //     Vector2 edge = Vector2Subtract(vertices[vertices.size() - 1], vertices[0]);
    //     Vector2 perpendiuclar = Vector2Normalize(Vector2{-edge.y, edge.x});
    //     Vector2 b = Vector2Add(Vector2Add(Vector2Scale(perpendiuclar, 100.0f), vertices[0]), Vector2Scale(edge, 0.5f));
    //     Vector2 a = Vector2Add(vertices[0], Vector2Scale(edge, 0.5f));
    //     DrawLineEx(a, b, 2.0f, BLUE);
    // }

    // for (size_t i = 1; i < vertices.size(); i++) {
    //     Vector2 edge = Vector2Subtract(vertices[i - 1], vertices[i]);
    //     Vector2 perpendiuclar = Vector2Normalize(Vector2{-edge.y, edge.x});
    //     Vector2 b = Vector2Add(Vector2Add(Vector2Scale(perpendiuclar, 100.0f), vertices[i]), Vector2Scale(edge, 0.5f));
    //     Vector2 a = Vector2Add(vertices[i], Vector2Scale(edge, 0.5f));
    //     DrawLineEx(a, b, 2.0f, BLUE);
    // }

    // for(int i = 0;i < vertices.size(); i++) {
    //     DrawText(TextFormat("%d", i), vertices[i].x, vertices[i].y, 40, BLUE);
    // }
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