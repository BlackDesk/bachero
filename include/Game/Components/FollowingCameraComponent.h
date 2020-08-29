#ifndef BACHERO_GAME_COMPONENTS_FOLLOWING_CAMERA_COMPONENTS
#define BACHERO_GAME_COMPONENTS_FOLLOWING_CAMERA_COMPONENTS

#include "Engine/ECS/ECS.h"
#include "Engine/Render/CameraComponent.h"
#include "Engine/Common/DeltaTime.h"

class FollowingCameraComponent : public Engine::ECS::Component {
public:
    explicit FollowingCameraComponent(Engine::ECS::Entity *target_ = nullptr)
            : target(target_) {}

    void init() override {
        _transform = owner->getComponent<Engine::TransformComponent>();
        _camera = owner->getComponent<Engine::Render::CameraComponent>();
    }

    void update() override {
        if (target && _transform && _camera) {
            float dt = std::min<float>(1.0f, Engine::DeltaTime::get());
            Engine::Math::Vector2f targetPos =
                    target->getComponent<Engine::TransformComponent>()->position;
            Engine::Math::Vector2f pos = _transform->position + _camera->getSize() / 2;
            _transform->position += (targetPos - pos) * dt * 4;
        }
    }

    Engine::ECS::Entity *target = nullptr;

private:
    Engine::TransformComponent *_transform = nullptr;
    Engine::Render::CameraComponent *_camera = nullptr;
};

#endif //BACHERO_GAME_COMPONENTS_FOLLOWING_CAMERA_COMPONENTS