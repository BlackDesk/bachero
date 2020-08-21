#ifndef BACHERO_ENGINE_COMPONENTS_TRANSFORM_COMPONENT
#define BACHERO_ENGINE_COMPONENTS_TRANSFORM_COMPONENT

#include "Engine/ECS/Component.h"
#include "Engine/Math/Vector2.h"

namespace Engine {
    class TransformComponent : public ECS::DataOnlyComponent {
    public:
        Math::Vector2d position;

        explicit TransformComponent(Math::Vector2d initialPos = {0, 0})
                : position(initialPos) {}
    };
}

#endif //BACHERO_ENGINE_COMPONENTS_TRANSFORM_COMPONENT