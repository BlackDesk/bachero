#ifndef BACHERO_ENGINE_PHYSICS_MANIFOLD
#define BACHERO_ENGINE_PHYSICS_MANIFOLD

#include "Engine/Physics/ColliderComponent.h"
#include "Engine/Physics/RigidBodyComponent.h"
#include "Engine/ECS/ECS.h"

namespace Engine::Physics {
    class Manifold {
    public:
        const ECS::Entity *bodyA = nullptr;
        const ECS::Entity *bodyB = nullptr;

        Manifold(ECS::Entity *A, ECS::Entity *B)
                : bodyA(A), bodyB(B) {
            rigidA = bodyA->getComponent<RigidBodyComponent>();
            rigidB = bodyB->getComponent<RigidBodyComponent>();

            transformA = bodyA->getComponent<TransformComponent>();
            transformB = bodyB->getComponent<TransformComponent>();

            boxA = bodyA->getComponent<ColliderComponent>()->getPositionedBB();
            boxB = bodyB->getComponent<ColliderComponent>()->getPositionedBB();

            dt = DeltaTime::get();
        }

        bool detect() {
            Math::Rect_d mBoxA = boxA;
            Math::Rect_d mBoxB = boxB;

//            mBoxA.position += rigidA->velocity * dt;
//            mBoxB.position += rigidB->velocity * dt;

            // Vector from A to B
            auto n = mBoxA.midPoint() - mBoxB.midPoint();

            // Calculate half extents along x axis for each object
            float a_extent_x = mBoxA.size.x / 2;
            float b_extent_x = mBoxB.size.x / 2;

            // Calculate overlap on x axis
            float x_overlap = a_extent_x + b_extent_x - abs(n.x);

            // SAT test on x axis
            if (x_overlap > 0) {
                // Calculate half extents along x axis for each object
                float a_extent_y = mBoxA.size.y / 2;
                float b_extent_y = mBoxB.size.y / 2;

                // Calculate overlap on y axis
                float y_overlap = a_extent_y + b_extent_y - abs(n.y);

                // SAT test on y axis
                if (y_overlap > 0) {
                    // Find out which axis is axis of least penetration
                    if (x_overlap < y_overlap) {
                        // Point towards B knowing that n points from A to B
                        normal = (n.x < 0 ?
                                  Math::Vector2d(-1, 0) :
                                  Math::Vector2d(1, 0));
                        penetration = x_overlap;
                    } else {
                        // Point toward B knowing that n points from A to B
                        normal = (n.y < 0 ?
                                  Math::Vector2d(0, -1) :
                                  Math::Vector2d(0, 1));
                        penetration = y_overlap;
                    }
                    return true;
                }
            }
            return false;
        }

        void positionalCorrection() {
            const float posCorrectionThreshold = 0;
            if (penetration < posCorrectionThreshold)
                return;
            if (rigidA->invMass + rigidB->invMass == 0)
                return;

            const float posCorrectionRate = 0.99;

            float correction = penetration * posCorrectionRate /
                               (rigidA->invMass + rigidB->invMass);

            Math::Vector2d correctionAlongNormal = normal * correction;

            transformA->position += correctionAlongNormal * rigidA->invMass;
            transformB->position -= correctionAlongNormal * rigidB->invMass;
        }

        void resolve() {
            if (rigidA->invMass + rigidA->invMass == 0)
                return;

            double velAlongNormal = (rigidB->velocity - rigidA->velocity).dotProduct(normal);

            if (velAlongNormal < 0)
                return;

            double e = std::min(rigidA->restitution, rigidB->restitution);
            double maxPenetration = std::max(boxA.size.x + boxB.size.x,
                                             boxA.size.y + boxB.size.y);
            double j = -(1.05 + e) * velAlongNormal * (1 + penetration / maxPenetration);

            j /= rigidA->invMass + rigidB->invMass;
            auto impulse = j * normal;

            rigidA->applyImpulse(-impulse);
            rigidB->applyImpulse(impulse);
        }

    private:
        double dt;

        RigidBodyComponent *rigidA;
        RigidBodyComponent *rigidB;

        TransformComponent *transformA;
        TransformComponent *transformB;

        Math::Rect_d boxA;
        Math::Rect_d boxB;

        Math::Vector2d normal;
        float penetration = 0;
    };
}

#endif //BACHERO_ENGINE_PHYSICS_MANIFOLD