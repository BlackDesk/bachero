#ifndef BACHERO_ENGINE_COMPONENTS_TRANSFORM_COMPONENT
#define BACHERO_ENGINE_COMPONENTS_TRANSFORM_COMPONENT

#include "Engine/ECS/Component.h"
#include "Engine/Math/Vector2.h"

namespace Engine {
    class TransformComponent : public ECS::DataOnlyComponent {
    public:
        Math::Vector2f position;
        float rotation = 0;

        explicit TransformComponent(Math::Vector2f initialPos = {0, 0},
                                    float initialRot = 0)
                : position(initialPos), rotation(initialRot) {}
    };
}

#endif //BACHERO_ENGINE_COMPONENTS_TRANSFORM_COMPONENT