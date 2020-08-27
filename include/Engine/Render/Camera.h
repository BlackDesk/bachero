#ifndef BACHERO_ENGINE_RENDER_CAMERA
#define BACHERO_ENGINE_RENDER_CAMERA

#include "Engine/ECS/ECS.h"
#include "Engine/Render/CameraComponent.h"

namespace Engine::Render {
    class Camera : public ECS::Entity {
    public:
        explicit Camera(Math::Vector2ui size = {100, 100}) {
            addComponent<TransformComponent>();
            addComponent<CameraComponent>(size);
        }
    };
}

#endif //BACHERO_ENGINE_RENDER_CAMERA