#ifndef BACHERO_GAME_COMPONENTS_PLAYER_MOVEMENT_COMPONENT
#define BACHERO_GAME_COMPONENTS_PLAYER_MOVEMENT_COMPONENT

#include "Engine/ECS/Component.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Input/InputMapper.h"
#include "Engine/Common/DeltaTime.h"

class PlayerMovementComponent : public Engine::ECS::Component {
public:
    void init() override {
        if (!owner->hasComponent<Engine::TransformComponent>())
            owner->addComponent<Engine::TransformComponent>();
        _transform = owner->getComponent<Engine::TransformComponent>();
    }

    void handleEvents() override {
        _movementDir = Engine::Input::InputMapper::getInstance()->getMovementDir();
    }

    void update() override {
        _transform->position += _movementDir * Engine::DeltaTime::get() * 200;
    }

private:
    Engine::Math::Vector2d _movementDir;
    Engine::TransformComponent *_transform = nullptr;
};

#endif //BACHERO_GAME_COMPONENTS_PLAYER_MOVEMENT_COMPONENT