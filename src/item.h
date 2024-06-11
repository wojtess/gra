#pragma once
#include "renderer.h"
#include <optional>
#include <functional>

namespace Entity {
    class Zombie;
    class Player;
}

namespace Items {
    class AbstractItem: public Renderer::Renderable {
        private:
        float pickupDistance;

        public:
        class Action {
            protected:
            float time;
            double started;
            std::function<void()> callback;
            public:
            Action(float time, std::function<void()> callback);
            float getRemainingTime();
            void callCallback();
        };

        AbstractItem(float pickupDistance);
        virtual void use(std::shared_ptr<Entity::Player>) = 0;
        int getUses();
        float getPickupDistance() const;
        std::optional<Action> getCurrentAction();
        void clearAction();
        virtual ~AbstractItem() = default;

        protected:
        int uses;
        std::optional<AbstractItem::Action> currentAction;
    };

    //GunItem is special one, beacuse you can shot somebody with it
    class GunItem: public AbstractItem {
        private:
        float reloadTime;
        float damage;
        std::optional<float> fireRate;
        int ammoCapacity;

        public:
        GunItem(float damage, int ammoCapacity, float reloadTime, float fireRate, float pickupDistance);
        GunItem(float damage, int ammoCapacity, float reloadTime);
        bool shoot(std::shared_ptr<Entity::Zombie>&);
        void use(std::shared_ptr<Entity::Player>) override;
        void render(Renderer::ResourceMap& resourceMap, bool flipped = false, bool effects = false) override;
        //firerate is used in game.cpp
        std::optional<float> getFireRate();

        protected:
        float lastShootTime;
    };

    class Pistol: public GunItem {
        private:
        public:
        Pistol();
        void render(Renderer::ResourceMap& resourceMap, bool flipped = false, bool effects = false) override;
    };

    class AkMachineGun: public GunItem {
        private:
        public:
        AkMachineGun();
        void render(Renderer::ResourceMap& resourceMap, bool flipped = false, bool effects = false) override;
    };

    class MedkitItem: public AbstractItem {
        public:
        MedkitItem();
        void use(std::shared_ptr<Entity::Player>) override;
        void render(Renderer::ResourceMap& resourceMap, bool flipped = false, bool effects = false) override;
    };
}