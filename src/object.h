#pragma once
#include "renderer.h"
#include <functional>
#include <string>
#include <vector>

namespace Shapes {

    class AbstractShape {
        public:
        virtual bool isColliding(const std::unique_ptr<AbstractShape>&) = 0;
        virtual std::vector<Vector2> getVertices() = 0;
        virtual void setPos(Vector2) = 0;
        virtual ~AbstractShape() = default;
    };

    class Circle: public AbstractShape {
        private:
        Vector2 pos;
        float radius;
        public:
        Circle(float radius);
        bool isColliding(const std::unique_ptr<AbstractShape>&);
        std::vector<Vector2> getVertices() override;
        void setPos(Vector2);
    };

    class Shape: public AbstractShape {
        private:
        std::vector<Vector2> vertices;
        Vector2 offset;
        public:
        Shape(std::vector<Vector2> vertices);
        bool isColliding(const std::unique_ptr<AbstractShape>&);
        std::vector<Vector2> getVertices() override;
        void setPos(Vector2);
    };

}

class PhysicsObject: public Renderer::Renderable2DObject {
    protected:
    Vector2 vel;
    Vector2 accel;
    std::vector<std::unique_ptr<Shapes::AbstractShape>> shapes;

    float dumpingFactor;
    void applyFrixion();

    public:
    PhysicsObject(float dumpingFactor);
    PhysicsObject();

    void setVel(Vector2);
    Vector2 getVel() const;
    void setAccel(Vector2);
    Vector2 getAccel() const;

    bool isColliding(const PhysicsObject& other);
    bool isColliding(const PhysicsObject* other);

    //dont pass any time, beacuse raylib have intergated function for that
    virtual void tick();
};

namespace Entity {
    class Zombie: public PhysicsObject {
        public:
        Zombie(Vector2 pos);
        void render() override;
    };
    class Player: public PhysicsObject {
        public:
        Player(Vector2 pos);
        void render() override;
    };

    class DropedItem: PhysicsObject {};
}

class Building: public PhysicsObject {
    private:
    Color color;
    public:
        Building(std::vector<Vector2> vertices, Color color);
        void render() override;
};

namespace Hud {
    class Button: public Renderer::Renderable {
        private:
            //onclick is just name in reality it is called on relase
            std::function<void()> onClick;
            std::string label;
            Vector2 pos;
            Vector2 size;
            Vector2 textPos;
            int fontSize;
            Renderer::Theme theme;
        public:
            Button(std::function<void()> onClick, std::string label, Vector2 pos, Vector2 size, int fontSize = 10, Renderer::Theme theme = Renderer::DEFAULT_TEHEME);
            void render() override;
    };

    class Label: public Renderer::Renderable {
        private:
            std::string label;
            Vector2 pos;
            float thick;
            Color color;
            int fontSize;
        public:
            Label(std::string label, Vector2 pos, Color color, int fontSize = 10);
            void render() override;
    };

}