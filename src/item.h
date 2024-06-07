#pragma once
#include "renderer.h"
#include <optional>

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
        std::optional<float> fireRate;
        int ammoCapacity;
        public:
        GunItem(float damage, int ammoCapacity, float fireRate);
        GunItem(float damage, int ammoCapacity);
        bool shoot(std::shared_ptr<Entity::Zombie>&);
        void use(std::shared_ptr<Entity::Player>) override;
        void render() override;
        //firerate is used in game.cpp
        std::optional<float> getFireRate();
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
        private:
        Texture2D tex;
        public:
        MedkitItem();
        ~MedkitItem();
        void use(std::shared_ptr<Entity::Player>) override;
        void render() override;
    };
}