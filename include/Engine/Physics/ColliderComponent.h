#ifndef BACHERO_ENGINE_PHYSICS_COLLIDER_COMPONENT
#define BACHERO_ENGINE_PHYSICS_COLLIDER_COMPONENT

#include "Engine/ECS/Component.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Math/Rect.h"
#include "Engine/Math/Mat2x2.h"

#include <climits>

//Rectangles only! Everything can be approximated with rectangles!
// Edges and vertices indexing (be attentive to the 0-1 indexing)
//    p0      e0     p1
//      |-----------|
//      |           |
//   e3 |           | e1
//      |           |
//      |-----------|
//    p3      e2     p2


namespace Engine::Physics {
    class ColliderComponent : public ECS::DataOnlyComponent {
    public:
        explicit ColliderComponent(Math::Rect_i boundingBox)
                : _boundingBox(boundingBox) {
            _centerOfMass = _boundingBox.midPoint();
        }

        void init() override {
            if (!owner->hasComponent<TransformComponent>())
                owner->addComponent<TransformComponent>();
            _transform = owner->getComponent<TransformComponent>();
        }

        Math::Rect_i getBoundingBox() const {
            return _boundingBox;
        }

        Math::Rect_f getPositionedBB() const {
            if (abs(_transform->rotation) <= eps) {
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

            if (abs(_transform->rotation) > eps) {
                Math::Mat2x2 mat = Math::getRotationalMat(_transform->rotation);
                p1 = mat * p1, p2 = mat * p2;
                p3 = mat * p3, p4 = mat * p4;
            }

            Math::Vector2f objPos = _transform->position;
            p1 += objPos, p2 += objPos;
            p3 += objPos, p4 += objPos;
        }

        void getCorners(Math::Vector2f c[]) const {
            getCorners(c[0], c[1], c[2], c[3]);
        }

        void getNormals(Math::Vector2f &n1, Math::Vector2f &n2,
                        Math::Vector2f &n3, Math::Vector2f &n4) const {
            n1 = {0, -1}, n2 = {1, 0};
            n3 = {0, 1}, n4 = {-1, 0};

            if (abs(_transform->rotation) <= eps)
                return;

            Math::Mat2x2 mat = Math::getRotationalMat(_transform->rotation);
            n1 = mat * n1, n2 = mat * n2;
            n3 = mat * n3, n4 = mat * n4;
        }

        void getNormals(Math::Vector2f n[]) const {
            getNormals(n[0], n[1], n[2], n[3]);
        }

        Math::Vector2f getSupportPoint(Math::Vector2f dir) const {
            Math::Vector2f p1, p2, p3, p4;
            getCorners(p1, p2, p3, p4);
            return getSupportPoint(p1, p2, p3, p4, dir);
        }

        Math::Vector2f getSupportPoint(Math::Vector2f p1, Math::Vector2f p2,
                                       Math::Vector2f p3, Math::Vector2f p4,
                                       Math::Vector2f dir) const {
            Math::Vector2f pts[] = {p1, p2, p3, p4};
            return getSupportPoint(pts, dir);
        }

        Math::Vector2f getSupportPoint(Math::Vector2f corners[],
                                       Math::Vector2f dir) const {
            float bestProjection = -FLT_MAX;
            Math::Vector2f bestPoint;

            for (std::size_t i = 0; i < 4; ++i) {
                auto pnt = corners[i];
                float projection = pnt.dotProduct(dir);
                if (projection > bestProjection) {
                    bestProjection = projection;
                    bestPoint = pnt;
                }
            }

            return bestPoint;
        }

        Math::Vector2f getAnchorPoint() const {
            return _transform->position;
        }

        Math::Vector2f getCenterOfMass() const {
            return _centerOfMass;
        }

    private:
        inline static float min4(float a, float b, float c, float d) {
            return std::min(std::min(a, b), std::min(c, d));
        }

        inline static float max4(float a, float b, float c, float d) {
            return std::max(std::max(a, b), std::max(c, d));
        }

        Math::Vector2f _centerOfMass;
        Math::Rect_i _boundingBox;
        TransformComponent *_transform = nullptr;

        const float eps = 1e-7;
    };
}

#endif