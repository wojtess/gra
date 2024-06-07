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

    GunItem::GunItem(float damage, float fireRate): AbstractItem(10.f), damage(damage), fireRate(fireRate) {
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

    Pistol::Pistol(): GunItem(0.5f, 1.0f) {
        tex = LoadTexture("../textures/gun1.png");
    }

    Pistol::~Pistol() {
        UnloadTexture(tex);
    }

    void Pistol::render() {
        DrawCircleV(Vector2{0.f, 0.f}, 10.0f, Color{0, 0, 0, 40});
        DrawTexturePro(tex, Rectangle{0,0,(float)tex.width,(float)tex.height}, Rectangle{0,0,20,20}, Vector2{10, 10}, 0.0f, WHITE);
    }

    AkMachineGun::AkMachineGun(): GunItem(1.5f, 0.2f) {
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