#include "item.h"
#include "object.h"

namespace Items {

    AbstractItem::Action::Action(float time, std::function<void()> callback): time(time), callback(callback ) {
        started = GetTime();
    }

    float AbstractItem::Action::getRemainingTime() {
        return time - (float)(GetTime() - started);
    }

    void AbstractItem::Action::callCallback() {
        callback();
    }

    AbstractItem::AbstractItem(float pickupDistance): pickupDistance(pickupDistance), currentAction({}) {}

    float AbstractItem::getPickupDistance() const {
        return pickupDistance;
    }

    void AbstractItem::clearAction() {
        currentAction = {};
    }

    std::optional<AbstractItem::Action> AbstractItem::getCurrentAction() {
        return currentAction;
    }

    int AbstractItem::getUses() {
        return uses;
    }

    GunItem::GunItem(float damage, int ammoCapacity, float reloadTime, float fireRate): AbstractItem(10.f), damage(damage), fireRate(fireRate), ammoCapacity(ammoCapacity), reloadTime(reloadTime) {
        uses = ammoCapacity;
        lastShootTime = GetTime();
    }

    GunItem::GunItem(float damage, int ammoCapacity, float reloadTime): AbstractItem(10.f), damage(damage), ammoCapacity(ammoCapacity), reloadTime(reloadTime) {
        uses = ammoCapacity;
    }

    bool GunItem::shoot(std::shared_ptr<Entity::Zombie>& other) {
        if (uses <= 0) {
            return false;
        };
        //somethink is going on with gun, so it can shoot now(reloading probably)
        if(currentAction) {
            return false;
        }
        //check if gun has firerate
        //if gun dont have firerate then it can only by shoot when button is relased and then pressed again
        //this is handled by game.cpp
        if(auto fireRateValue = fireRate) {
            //if gun has firerate then dont shoot if last shoot is faster that allowed by fireRate
            if(GetTime() - lastShootTime < 1.0f/ *fireRateValue) {
                return false;
            }
        }

        lastShootTime = GetTime();
        if (other) {
            other->setHp(other->getHp() - damage);
        };
        uses--;
        return true;
    }

    void GunItem::use(std::shared_ptr<Entity::Player> player) {
        if(currentAction) {
            return;
        }
        currentAction = AbstractItem::Action(reloadTime, [this]() {
            uses = ammoCapacity;
        });
    }

    void GunItem::render(Renderer::ResourceMap& resourceMap) {
        DrawCircleV(Vector2{0.f, 0.f}, 10.0f, GREEN);
    }

    std::optional<float> GunItem::getFireRate() {
        return fireRate;
    }

    Pistol::Pistol(): GunItem(0.5f, 17, 0.5f) {
    }

    void Pistol::render(Renderer::ResourceMap& resourceMap) {
        DrawCircleV(Vector2{0.f, 0.f}, 10.0f, Color{0, 0, 0, 40});
        auto tex = resourceMap.getTexture("gun1.png");
        DrawTexturePro(tex, Rectangle{0,0,(float)tex.width,(float)tex.height}, Rectangle{0,0,20,20}, Vector2{10, 10}, 0.0f, WHITE);
    }

    AkMachineGun::AkMachineGun(): GunItem(2.5f, 30, 3.5f, 10.0f) {
    }

    void AkMachineGun::render(Renderer::ResourceMap& resourceMap) {
        DrawCircleV(Vector2{0.f, 0.f}, 10.0f, Color{0, 0, 0, 40});
        auto tex = resourceMap.getTexture("ak47.png");
        DrawTexturePro(tex, Rectangle{0,0,(float)tex.width,(float)tex.height}, Rectangle{0,0,20,20}, Vector2{10, 10}, 0.0f, WHITE);
    }

    MedkitItem::MedkitItem(): AbstractItem(10.f) {
    }

    void MedkitItem::use(std::shared_ptr<Entity::Player> player) {
        if(player->getHp() >= 100) {
            return;
        }
        player->setHp(100);
        player->getItems()[player->getSelctedItemIndex()] = std::shared_ptr<Items::AbstractItem>();
    }

    void MedkitItem::render(Renderer::ResourceMap& resourceMap) {
        DrawCircleV(Vector2{0.f, 0.f}, 10.0f, Color{0, 0, 0, 40});
        auto tex = resourceMap.getTexture("med.png");
        DrawTexturePro(tex, Rectangle{0,0,(float)tex.width,(float)tex.height}, Rectangle{0,0,20,20}, Vector2{10, 10}, 0.0f, WHITE);
    }
}