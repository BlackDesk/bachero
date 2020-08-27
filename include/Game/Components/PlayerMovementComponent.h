#ifndef BACHERO_GAME_COMPONENTS_PLAYER_MOVEMENT_COMPONENT
#define BACHERO_GAME_COMPONENTS_PLAYER_MOVEMENT_COMPONENT

#include "Engine/ECS/Component.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Physics/RigidBodyComponent.h"
#include "Engine/Input/InputMapper.h"
#include "Engine/Common/DeltaTime.h"

#include "Game/Components/PlayerAnimationComponent.h"

class PlayerMovementComponent : public Engine::ECS::Component {
public:
    void init() override {
        if (!owner->hasComponent<Engine::TransformComponent>())
            owner->addComponent<Engine::TransformComponent>();
        _transform = owner->getComponent<Engine::TransformComponent>();
        _rigidBody = owner->getComponent<Engine::Physics::RigidBodyComponent>();
        _animation = owner->getComponent<PlayerAnimationComponent>();
    }

    void handleEvents() override {
//        _rigidBody->applyAcceleration(Engine::Input::InputMapper::getInstance()->getMovementDir() * 10);
        auto dir = Engine::Input::InputMapper::getInstance()->getMovementDir();
        _rigidBody->velocity = dir * 80;

        PlayerAnimationComponent::Direction direction = _animation->direction;
        PlayerAnimationComponent::State state = _animation->state;
        if (dir.length() > 0.05) {
            state = PlayerAnimationComponent::State::walking;
            if (abs(dir.x) >= abs(dir.y)) {
                if (dir.x < 0)
                    direction = PlayerAnimationComponent::Direction::left;
                else
                    direction = PlayerAnimationComponent::Direction::right;
            } else {
                if (dir.y < 0)
                    direction = PlayerAnimationComponent::Direction::up;
                else
                    direction = PlayerAnimationComponent::Direction::down;
            }
        } else {
            state = PlayerAnimationComponent::State::idle;
        }

        _animation->playerSpeed = dir.length();
        if (direction != _animation->direction ||
            state != _animation->state) {
            _animation->state = state;
            _animation->direction = direction;
            _animation->currentFrame = 0;
        }
    }

    void update() override {

    }

    void render() override {
        if (Engine::Input::InputManager::getInstance()->onKeyUp(SDL_SCANCODE_R))
            owner->destroy();
    }

private:
    PlayerAnimationComponent *_animation = nullptr;
    Engine::TransformComponent *_transform = nullptr;
    Engine::Physics::RigidBodyComponent *_rigidBody = nullptr;
};

#endif //BACHERO_GAME_COMPONENTS_PLAYER_MOVEMENT_COMPONENT