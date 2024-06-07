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
        int getUses();
        float getPickupDistance() const;
        virtual ~AbstractItem() = default;
        protected:
        int uses;
    };

    //GunItem is special one, beacuse you can shot somebody with it
    class GunItem: public AbstractItem {
        private:
        float damage;
        float fireRate;
        public:
        GunItem(float damage, float fireRate);
        bool shoot(std::shared_ptr<Entity::Zombie>&);
        void use(std::shared_ptr<Entity::Player>) override;
        void render() override;
    };

    class Pistol: public GunItem {
        private:
        Texture2D tex;
        public:
        Pistol();
        ~Pistol();
        void render() override;
    };

    class AkMachineGun: public GunItem {
        private:
        Texture2D tex;
        public:
        AkMachineGun();
        ~AkMachineGun();
        void render() override;
    };

    class MedkitItem: public AbstractItem {
        public:
        MedkitItem();
        void use(std::shared_ptr<Entity::Player>) override;
        void render() override;
    };
}