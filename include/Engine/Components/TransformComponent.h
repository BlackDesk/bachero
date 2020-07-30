#ifndef BACHERO_ENGINE_COMPONENTS_TRANSFORM_COMPONENT
#define BACHERO_ENGINE_COMPONENTS_TRANSFORM_COMPONENT

#include "Engine/ECS/Component.h"
#include "Engine/Math/Vector2.h"

namespace Engine {
    class TransformComponent : public ECS::Component {
    public:
        Math::Vector2d position;
    };
}

#endif //BACHERO_ENGINE_COMPONENTS_TRANSFORM_COMPONENT