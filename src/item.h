#pragma once
#include "renderer.h"

namespace Entity {
    class Zombie;
    class Player;
}

namespace Items {
    class AbstractItem: public Renderer::Renderable {
        public:
        virtual void use(std::shared_ptr<Entity::Player>) = 0;
        virtual ~AbstractItem() = default;
    };

    //GunItem is special one, beacuse you can shot somebody with it
    class GunItem: public AbstractItem {
        public:
        void shoot(std::unique_ptr<Entity::Zombie>&);
        void use(std::shared_ptr<Entity::Player>) override;
        void render() override;
    };
}