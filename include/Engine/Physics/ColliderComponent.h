#ifndef BACHERO_ENGINE_PHYSICS_COLLIDER_COMPONENT
#define BACHERO_ENGINE_PHYSICS_COLLIDER_COMPONENT

#include "Engine/ECS/Component.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Math/Rect.h"

namespace Engine::Physics {
    class ColliderComponent : public ECS::DataOnlyComponent {
    public:
        explicit ColliderComponent(Math::Rect_i boundingBox)
                : _boundingBox(boundingBox) {}

        void init() override {
            if (!owner->hasComponent<TransformComponent>())
                owner->addComponent<TransformComponent>();
            _transform = owner->getComponent<TransformComponent>();
        }

        Math::Rect_i getBoundingBox() const {
            return _boundingBox;
        }

        Math::Rect_i getPositionedBB() const {
            Math::Rect_i bb = _boundingBox;
            bb.position += _transform->position;
            return bb;
        }

    private:
        Math::Rect_i _boundingBox;
        TransformComponent *_transform = nullptr;
    };
}

#endif