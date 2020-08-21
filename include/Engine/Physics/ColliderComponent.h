#ifndef BACHERO_ENGINE_PHYSICS_COLLIDER_COMPONENT
#define BACHERO_ENGINE_PHYSICS_COLLIDER_COMPONENT

#include "Engine/ECS/Component.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Math/Rect.h"
#include "Engine/Render/TextureManager.h"

namespace Engine::Physics {
    class ColliderComponent : public ECS::DataOnlyComponent {
    public:
        explicit ColliderComponent(Math::Rect_i boundingBox)
                : _boundingBox(boundingBox) {}

        Math::Rect_i getBoundingBox() {
            return _boundingBox;
        }

    private:
        Math::Rect_i _boundingBox;
    };
}

#endif