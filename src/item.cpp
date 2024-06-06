#include "item.h"
#include "object.h"

namespace Items {

    AbstractItem::AbstractItem(float pickupDistance): pickupDistance(pickupDistance) {}

    float AbstractItem::getPickupDistance() const {
        return pickupDistance;
    }

    int AbstractItem::getUses() {
        return uses;
    }

    GunItem::GunItem(): AbstractItem(10.f) {
        uses = 10;
    }

    bool GunItem::shoot(std::shared_ptr<Entity::Zombie>& other) {
        if (uses <= 0) {
            return false;
        };
        if (other) {
            other->setHp(other->getHp() - 1.0f);
        };
        uses--;
        return true;
    }

    void GunItem::use(std::shared_ptr<Entity::Player> player) {
        uses = 10;
    }

    void GunItem::render() {
        DrawCircleV(Vector2{0.f, 0.f}, 10.0f, GREEN);
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

    void MedkitItem::render() {
        DrawCircleV(Vector2{0.f, 0.f}, 10.0f, BLUE);
    }
}