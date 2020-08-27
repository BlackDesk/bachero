#ifndef BACHERO_GAME_PLAYER
#define BACHERO_GAME_PLAYER

#include "Game/Components/PlayerMovementComponent.h"
#include "Game/Components/PlayerAnimationComponent.h"
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
                loadTexture("assets/player.png",
                            "player");
        auto *transform = addComponent<Engine::TransformComponent>(
                Engine::Math::Vector2f(100, 100));
        auto *sprite = addComponent<Engine::Render::SpriteComponent>(
                texture,
                Engine::Math::Vector2ui(32, 32));
        sprite->position = {-12 - 4, -26 - 2};
        sprite->frame = {1, 0};

        addComponent<PlayerMovementComponent>();
        addComponent<PlayerAnimationComponent>();
        addComponent<Engine::Physics::ColliderComponent>(Engine::Math::Rect_i{
                Engine::Math::Vector2i(-4, -2),
                Engine::Math::Vector2ui(8, 4),
        });
        auto *rigid = addComponent<Engine::Physics::RigidBodyComponent>(
                60, 0, 0.5);
        rigid->lockRotation();
        const_cast<float &>(rigid->invI) = 0.0f;
    }

private:
};

#endif //BACHERO_GAME_PLAYER