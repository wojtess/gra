#pragma once
#include "renderer.h"

namespace Entity {
    class Zombie;
    class Player;
}

namespace Items {
    class AbstractItem: public Renderer::Renderable {
        private:
        float pickupDistance;
        public:
        AbstractItem(float pickupDistance);
        virtual void use(std::shared_ptr<Entity::Player>) = 0;
        float getPickupDistance() const;
        virtual ~AbstractItem() = default;
    };

    //GunItem is special one, beacuse you can shot somebody with it
    class GunItem: public AbstractItem {
        public:
        GunItem();
        void shoot(std::unique_ptr<Entity::Zombie>&);
        void use(std::shared_ptr<Entity::Player>) override;
        void render() override;
    };
}