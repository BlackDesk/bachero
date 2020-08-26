#ifndef BACHERO_GAME_PLAYER
#define BACHERO_GAME_PLAYER

#include "Game/Components/PlayerMovementComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Render/SpriteComponent.h"
#include "Engine/Render/TextureManager.h"
#include "Engine/Physics/ColliderComponent.h"
#include "Engine/Physics/RigidBodyComponent.h"
#include "Engine/ECS/Entity.h"

class Player : public Engine::ECS::Entity {
public:
    explicit Player() {
        auto *texture = Engine::Render::TextureManager::getInstance()->
                loadTexture("assets/sprite.png",
                            "player");
        auto *transform = addComponent<Engine::TransformComponent>(
                Engine::Math::Vector2f(100, 100));
        auto *sprite = addComponent<Engine::Render::SpriteComponent>(
                texture,
                Engine::Math::Vector2ui(144, 192));
        sprite->position = {-50, 0};
        sprite->frame = {1, 0};

        addComponent<PlayerMovementComponent>();
        addComponent<Engine::Physics::ColliderComponent>(Engine::Math::Rect_i{
                Engine::Math::Vector2i(-20, -10),
                Engine::Math::Vector2ui(104, 30),
        });
        auto *rigid = addComponent<Engine::Physics::RigidBodyComponent>(
                500, 0, 0.5);
        rigid->lockRotation();
        const_cast<float &>(rigid->invI) = 0.0f;
    }

private:
};

#endif //BACHERO_GAME_PLAYER