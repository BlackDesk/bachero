#ifndef BACHERO_GAME_ENTITIES_BOX
#define BACHERO_GAME_ENTITIES_BOX

#include "Engine/Components/TransformComponent.h"
#include "Engine/Physics/ColliderComponent.h"
#include "Engine/Physics/RigidBodyComponent.h"

class Box : public Engine::ECS::Entity {
public:
    explicit Box(Engine::Math::Vector2d position = {0, 0}) {
        addComponent<Engine::TransformComponent>(position);
        addComponent<Engine::Physics::ColliderComponent>(Engine::Math::Rect_i{
                Engine::Math::Vector2i(0, 0),
                Engine::Math::Vector2ui(50, 50),
        });
        addComponent<Engine::Physics::RigidBodyComponent>(20, 0.5, 0.5);
    }

private:
};

#endif //BACHERO_GAME_ENTITIES_BOX