#ifndef BACHERO_ENGINE_FOLLOWING_CAMERA
#define BACHERO_ENGINE_FOLLOWING_CAMERA

#include "Engine/ECS/ECS.h"
#include "Engine/Render/CameraComponent.h"
#include "Game/Components/FollowingCameraComponent.h"

class FollowingCamera : public Engine::ECS::Entity {
public:
    explicit FollowingCamera(Engine::Math::Vector2ui size,
                    Engine::ECS::Entity *target = nullptr) {
        addComponent<Engine::TransformComponent>();
        addComponent<Engine::Render::CameraComponent>(size);
        addComponent<FollowingCameraComponent>(target);
    }
};

#endif //BACHERO_ENGINE_FOLLOWING_CAMERA