#include "item.h"
#include "object.h"

namespace Items {

    AbstractItem::AbstractItem(float pickupDistance): pickupDistance(pickupDistance) {}

    float AbstractItem::getPickupDistance() const {
        return pickupDistance;
    }

    GunItem::GunItem(): AbstractItem(22.f) {}

    void GunItem::shoot(std::unique_ptr<Entity::Zombie>& other) {

    }

    void GunItem::use(std::shared_ptr<Entity::Player> player) {

    }

    void GunItem::render() {
        DrawCircleV(Vector2{0.f, 0.f}, 10.0f, GREEN);
    }
}