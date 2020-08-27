#ifndef BACHERO_GAME_ENTITIES_BOX
#define BACHERO_GAME_ENTITIES_BOX

#include "Engine/Components/TransformComponent.h"
#include "Engine/Physics/ColliderComponent.h"
#include "Engine/Physics/RigidBodyComponent.h"

class Box : public Engine::ECS::Entity {
public:
    explicit Box(Engine::Math::Vector2d position = {0, 0}) {
        auto *texture = Engine::Render::TextureManager::getInstance()->
                loadTexture("assets/crate.png",
                            "crate");
        Engine::Math::Vector2i size(15, 15);
        auto *sprite = addComponent<Engine::Render::SpriteComponent>(
                texture);
        sprite->position = {-7, -7};

        addComponent<Engine::TransformComponent>(position + size / 2);
        addComponent<Engine::Physics::ColliderComponent>(Engine::Math::Rect_i{
                -size / 2,
                size,
        });
        addComponent<Engine::Physics::RigidBodyComponent>(20, 0.5, 0.5);
    }

private:
};

#endif //BACHERO_GAME_ENTITIES_BOX