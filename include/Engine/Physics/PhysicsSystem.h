#ifndef BACHERO_ENGINE_PHYSICS_SYSTEM
#define BACHERO_ENGINE_PHYSICS_SYSTEM

#include "Engine/Physics/ColliderComponent.h"
#include "Engine/Physics/RigidBodyComponent.h"
#include "Engine/Render/TextureManager.h"
#include "Engine/Render/Text.h"
#include "Engine/ECS/ECS.h"
#include "Engine/Common/DeltaTime.h"

#include <iostream>

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

            boxA = bodyA->getComponent<ColliderComponent>()->getBoundingBox();
            boxB = bodyB->getComponent<ColliderComponent>()->getBoundingBox();

            boxA.position += transformA->position;
            boxB.position += transformB->position;

            dt = DeltaTime::get();
        }

        bool detect() {
            Math::Rect_d mBoxA = boxA;
            Math::Rect_d mBoxB = boxB;

            mBoxA.position += rigidA->velocity * dt;
            mBoxB.position += rigidB->velocity * dt;

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

            const float posCorrectionRate = 0.8;

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
            double j = -(1 + e) * velAlongNormal;

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

    class PhysicsSystem : public ECS::System {
    public:
        void update() override {
            _collidersToDraw.clear();

            auto dt = DeltaTime::get();

            auto bodies = ECS::EntityManager::getInstance()->
                    getEntitiesThatHaveComponent<RigidBodyComponent>();

            for (auto &body : bodies) {
                auto *rigid = body->getComponent<RigidBodyComponent>();
//                rigid->applyAcceleration({0, 100});
                rigid->integrateAcceleration(dt);
            }

            _collidersToDraw.resize(bodies.size());
            for (size_t i = 0; i < bodies.size(); ++i) {
                auto &bodyA = bodies[i];
                for (size_t j = i + 1; j < bodies.size(); ++j) {
                    auto &bodyB = bodies[j];
                    Manifold manifold(bodyA, bodyB);
                    if (manifold.detect()) {
                        std::get<1>(_collidersToDraw[i]) = true;
                        std::get<1>(_collidersToDraw[j]) = true;
                        manifold.resolve();
                    }
                }
            }

            for (auto &body : bodies) {
                auto *rigid = body->getComponent<RigidBodyComponent>();
                rigid->integrateVelocity(dt);
            }

            for (int iter = 0; iter < 5; ++iter) {
                for (size_t i = 0; i < bodies.size(); ++i) {
                    auto &bodyA = bodies[i];
                    for (size_t j = i + 1; j < bodies.size(); ++j) {
                        auto &bodyB = bodies[j];
                        Manifold manifold(bodyA, bodyB);
                        if (manifold.detect())
                            manifold.positionalCorrection();
                    }
                }
            }
            for (size_t i = 0; i < bodies.size(); ++i) {
                auto &body = bodies[i];
                auto *rigid = body->getComponent<RigidBodyComponent>();

                Math::Rect_d box = body->getComponent<ColliderComponent>()->getBoundingBox();
                box.position += body->getComponent<TransformComponent>()->position;

                auto box_ = (SDL_Rect) box;
                rigid->acceleration = {0, 0};
                std::get<0>(_collidersToDraw[i]) = box_;
            }
        }

        void render() override {
            auto *renderer = Render::TextureManager::getInstance()->getDefaultRenderer();
            auto *sdlRenderer = renderer->get();
            for (auto[rect, color] : _collidersToDraw) {
                if (color)
                    SDL_SetRenderDrawColor(
                            sdlRenderer,
                            255, 0, 0, 255);
                else
                    SDL_SetRenderDrawColor(
                            sdlRenderer,
                            0, 255, 0, 255);

                SDL_RenderDrawRect(
                        sdlRenderer,
                        &rect);
            }
        }

    private:
        std::vector<std::tuple<SDL_Rect, bool>> _collidersToDraw;

        //Thanks to OLC: https://www.youtube.com/watch?v=8JJ-4JgR7Dg
        static bool rayVsBoxCollisionDetection(Math::Vector2d rayOrigin,
                                               Math::Vector2d rayDirection,
                                               Math::Rect_d target,
                                               Math::Vector2d &contactPoint,
                                               Math::Vector2d &contactNormal,
                                               double &t_hitNear) {
            Math::Vector2d t_near = (target.position - rayOrigin)
                    .elementWiseDivision(rayDirection);
            Math::Vector2d t_far = (target.position + target.size - rayOrigin)
                    .elementWiseDivision(rayDirection);

            if (t_near.x > t_far.x)
                std::swap(t_near.x, t_far.x);
            if (t_near.y > t_far.y)
                std::swap(t_near.y, t_far.y);

            if (t_near.x > t_far.y || t_near.y > t_far.x)
                return false;

            t_hitNear = std::max(t_near.x, t_near.y);
            double t_hitFar = std::max(t_far.x, t_far.y);

            if (t_hitFar < 0 || t_hitNear > 1)
                return false;

            contactPoint = rayOrigin + t_hitNear * rayDirection;

            if (t_near.x > t_near.y) {
                if (rayDirection.x < 0)
                    contactNormal = {1, 0};
                else
                    contactNormal = {-1, 0};
            } else {
                if (rayDirection.x < 0)
                    contactNormal = {0, 1};
                else
                    contactNormal = {0, -1};
            }

            return true;
        }
    };
}

#endif //BACHERO_ENGINE_PHYSICS_SYSTEM