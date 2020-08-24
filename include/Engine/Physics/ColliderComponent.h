#ifndef BACHERO_ENGINE_PHYSICS_COLLIDER_COMPONENT
#define BACHERO_ENGINE_PHYSICS_COLLIDER_COMPONENT

#include "Engine/ECS/Component.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Math/Rect.h"
#include "Engine/Math/Mat2x2.h"

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

        Math::Rect_f getPositionedBB() const {
            if (abs(_transform->rotation) <= 1e-7) {
                auto bb = _boundingBox;
                bb.position += _transform->position;
                return bb;
            }

            Math::Vector2f p1, p2, p3, p4;
            getCorners(p1, p2, p3, p4);

            Math::Vector2f bp1(min4(p1.x, p2.x, p3.x, p4.x),
                               min4(p1.y, p2.y, p3.y, p4.y));
            Math::Vector2f bp2(max4(p1.x, p2.x, p3.x, p4.x),
                               max4(p1.y, p2.y, p3.y, p4.y));

            return Math::Rect_f(bp1, bp2 - bp1);
        }

        void getCorners(Math::Vector2f &p1, Math::Vector2f &p2,
                        Math::Vector2f &p3, Math::Vector2f &p4) const {
            Math::Rect_i bb = _boundingBox;

            bb.getCorners(p1, p2, p3, p4);

            Math::Mat2x2 mat = Math::getRotationalMat(_transform->rotation);
            Math::Vector2f objPos = _transform->position;

            p1 = mat * p1 + objPos;
            p2 = mat * p2 + objPos;
            p3 = mat * p3 + objPos;
            p4 = mat * p4 + objPos;
        }

        Math::Vector2f getAnchorPoint() const {
            return _transform->position;
        }

    private:
        inline static float min4(float a, float b, float c, float d) {
            return std::min(std::min(a, b), std::min(c, d));
        }

        inline static float max4(float a, float b, float c, float d) {
            return std::max(std::max(a, b), std::max(c, d));
        }

        Math::Rect_i _boundingBox;
        TransformComponent *_transform = nullptr;
    };
}

#endif