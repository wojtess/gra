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

    GunItem::GunItem(float damage, int ammoCapacity, float fireRate): AbstractItem(10.f), damage(damage), fireRate(fireRate), ammoCapacity(ammoCapacity) {
        uses = ammoCapacity;
    }

    GunItem::GunItem(float damage, int ammoCapacity): AbstractItem(10.f), damage(damage), ammoCapacity(ammoCapacity) {
        uses = ammoCapacity;
    }

    bool GunItem::shoot(std::shared_ptr<Entity::Zombie>& other) {
        if (uses <= 0) {
            return false;
        };
        if (other) {
            other->setHp(other->getHp() - damage);
        };
        uses--;
        return true;
    }

    void GunItem::use(std::shared_ptr<Entity::Player> player) {
        uses = ammoCapacity;
    }

    void GunItem::render() {
        DrawCircleV(Vector2{0.f, 0.f}, 10.0f, GREEN);
    }

    Pistol::Pistol(): GunItem(0.5f, 17) {
        tex = LoadTexture("../textures/gun1.png");
    }

    Pistol::~Pistol() {
        UnloadTexture(tex);
    }

    void Pistol::render() {
        DrawCircleV(Vector2{0.f, 0.f}, 10.0f, Color{0, 0, 0, 40});
        DrawTexturePro(tex, Rectangle{0,0,(float)tex.width,(float)tex.height}, Rectangle{0,0,20,20}, Vector2{10, 10}, 0.0f, WHITE);
    }

    AkMachineGun::AkMachineGun(): GunItem(2.5f, 30, 1.0f) {
        tex = LoadTexture("../textures/ak47.png");
    }

    AkMachineGun::~AkMachineGun() {
        UnloadTexture(tex);
    }

    void AkMachineGun::render() {
        DrawCircleV(Vector2{0.f, 0.f}, 10.0f, Color{0, 0, 0, 40});
        DrawTexturePro(tex, Rectangle{0,0,(float)tex.width,(float)tex.height}, Rectangle{0,0,20,20}, Vector2{10, 10}, 0.0f, WHITE);
    }

    MedkitItem::MedkitItem(): AbstractItem(10.f) {
        tex = LoadTexture("../textures/med.png");
    }

    MedkitItem::~MedkitItem() {
        UnloadTexture(tex);
    }

    void MedkitItem::use(std::shared_ptr<Entity::Player> player) {
        if(player->getHp() >= 100) {
            return;
        }
        player->setHp(100);
        player->getItems()[player->getSelctedItemIndex()] = std::shared_ptr<Items::AbstractItem>();
    }

    void MedkitItem::render() {
        DrawCircleV(Vector2{0.f, 0.f}, 10.0f, Color{0, 0, 0, 40});
        DrawTexturePro(tex, Rectangle{0,0,(float)tex.width,(float)tex.height}, Rectangle{0,0,20,20}, Vector2{10, 10}, 0.0f, WHITE);
    }
}