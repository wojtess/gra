#pragma once
#include "renderer.h"
#include "item.h"
#include <functional>
#include <string>
#include <vector>
#include <optional>
#include "constants.h"

//forward declaration
class Game;

namespace Shapes {

    class AbstractShape {
        public:
        virtual std::optional<Vector2> isColliding(const std::shared_ptr<AbstractShape>&) = 0;
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
        std::optional<Vector2> isColliding(const std::shared_ptr<AbstractShape>&);
        std::vector<Vector2> getVertices() override;
        void setPos(Vector2);
        Vector2 getPos() const;
        float getRadius() const;
    };

    class Shape: public AbstractShape {
        private:
        std::vector<Vector2> vertices;
        Vector2 offset;
        public:
        Shape(std::vector<Vector2> vertices);
        std::optional<Vector2> isColliding(const std::shared_ptr<AbstractShape>&);
        std::vector<Vector2> getVertices() override;
        void setPos(Vector2);
    };

}

class PhysicsObject: public Renderer::Renderable2DObject {
    protected:
    Vector2 vel;
    Vector2 accel;
    std::vector<std::shared_ptr<Shapes::AbstractShape>> shapes;
    bool physicsOn;

    float dumpingFactor;
    void applyFrixion();

    public:
    PhysicsObject(float dumpingFactor);
    PhysicsObject();
    virtual ~PhysicsObject() = default;

    void setVel(Vector2);
    Vector2 getVel() const;
    void setAccel(Vector2);
    Vector2 getAccel() const;

    bool isColliding(const PhysicsObject& other);
    bool isColliding(const PhysicsObject* other);
    bool isColliding(const std::shared_ptr<PhysicsObject> other);

    std::optional<Vector2> getIntersectionPoint(Vector2 origin, Vector2 end);

    //dont pass any time, beacuse raylib have intergated function for that
    virtual void tick(Game&);
};

namespace Entity {
    class Zombie: public PhysicsObject {
        private:
        float speed;

        int hp;
        public:
        Zombie(Vector2 pos, float speed);
        void render(Renderer::ResourceMap& resourceMap, bool flipped = false, bool effects = false) override;
        //tick is overrided, beacuse zombie need to have "ai" that will follow player
        void tick(Game&) override;
        void setHp(int nowe);
        int getHp();
    };

    class Player: public PhysicsObject {
        private:
        //lookingDirection is normalized value, only direction is import so this could be saved as one float value, but for easier implementation it is stored as vector2
        Vector2 lookingDirection;
        std::array<std::shared_ptr<Items::AbstractItem>, STACK_SIZE> items;
        int selectedItem;

        float hp;

        public:
        Player(Vector2 pos);
        void render(Renderer::ResourceMap& resourceMap, bool flipped = false, bool effects = false) override;

        std::array<std::shared_ptr<Items::AbstractItem>, STACK_SIZE>& getItems();
        int getSelctedItemIndex();
        void incraseSelectedItemIndex();
        void decraseSelectedItemIndex();
        std::optional<std::shared_ptr<Items::AbstractItem>> getSelectedItem();

        void setLookingDirection(Vector2);
        Vector2 getLookingDirection() const;

        void setHp(float nowe);
        float getHp();
    };

    class DropedItem: public PhysicsObject {
        private:
        std::shared_ptr<Items::AbstractItem> item;
        public:
        DropedItem(std::shared_ptr<Items::AbstractItem>, Vector2);
        void render(Renderer::ResourceMap& resourceMap, bool flipped = false, bool effects = false) override;
        std::shared_ptr<Items::AbstractItem>& getItem();
    };
}

class Building: public PhysicsObject {
    private:
    Color color;
    public:
        Building(std::vector<Vector2> vertices, Color color);
        Building(std::vector<Vector2> vertices, Color color, Vector2 pos);
        void render(Renderer::ResourceMap& resourceMap, bool flipped = false, bool effects = false) override;
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
            void render(Renderer::ResourceMap& resourceMap, bool flipped = false, bool effects = false) override;
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
            void render(Renderer::ResourceMap& resourceMap, bool flipped = false, bool effects = false) override;
    };
}