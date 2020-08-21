#ifndef BACHERO_GAME_ENTITIES_WALL
#define BACHERO_GAME_ENTITIES_WALL

#include "Engine/Components/TransformComponent.h"
#include "Engine/Physics/ColliderComponent.h"
#include "Engine/Physics/RigidBodyComponent.h"

class Wall : public Engine::ECS::Entity {
public:
    explicit Wall(Engine::Math::Rect_i rect = {Engine::Math::Vector2i(200, 100),
                                               Engine::Math::Vector2ui(50, 100)
    }) {
        addComponent<Engine::TransformComponent>(rect.position);
        addComponent<Engine::Physics::ColliderComponent>(Engine::Math::Rect_i{
                Engine::Math::Vector2i(0, 0),
                rect.size
        });
        addComponent<Engine::Physics::RigidBodyComponent>(0, 0.5, 0.5);
    }

private:
};

#endif //BACHERO_GAME_ENTITIES_BOX