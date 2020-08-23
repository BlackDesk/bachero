#ifndef BACHERO_GAME_COMPONENTS_PLAYER_MOVEMENT_COMPONENT
#define BACHERO_GAME_COMPONENTS_PLAYER_MOVEMENT_COMPONENT

#include "Engine/ECS/Component.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Physics/RigidBodyComponent.h"
#include "Engine/Input/InputMapper.h"
#include "Engine/Common/DeltaTime.h"

class PlayerMovementComponent : public Engine::ECS::Component {
public:
    void init() override {
        if (!owner->hasComponent<Engine::TransformComponent>())
            owner->addComponent<Engine::TransformComponent>();
        if (!owner->hasComponent<Engine::Physics::RigidBodyComponent>())
            throw std::runtime_error("PlayerMovement requires RigidBody.");
        _transform = owner->getComponent<Engine::TransformComponent>();
        _rigidBody = owner->getComponent<Engine::Physics::RigidBodyComponent>();
    }

    void handleEvents() override {
//        _rigidBody->applyAcceleration(Engine::Input::InputMapper::getInstance()->getMovementDir() * 10);
        _rigidBody->velocity = Engine::Input::InputMapper::getInstance()->getMovementDir() * 200;
        int lArr = Engine::Input::InputManager::getInstance()->isKeyDown(SDL_SCANCODE_K);
        int rArr = Engine::Input::InputManager::getInstance()->isKeyDown(SDL_SCANCODE_L);
        _rigidBody->angularVelocity = (rArr - lArr) * 1.0f;
    }

    void update() override {

    }

private:
    Engine::TransformComponent *_transform = nullptr;
    Engine::Physics::RigidBodyComponent *_rigidBody = nullptr;
};

#endif //BACHERO_GAME_COMPONENTS_PLAYER_MOVEMENT_COMPONENT