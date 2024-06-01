#pragma once
#include "renderer.h"
#include <functional>
#include <string>

class PhysicsObject: public Renderer::Renderable2DObject {
    protected:
    Vector2 vel;
    float dumpingFactor;
    void applyFrixion();

    public:
    PhysicsObject(float dumpingFactor);
    PhysicsObject();

    void setVel(Vector2);
    Vector2 getVel() const;

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