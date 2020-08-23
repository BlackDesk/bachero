#ifndef BACHERO_ENGINE_PHYSICS_SYSTEM
#define BACHERO_ENGINE_PHYSICS_SYSTEM

#include "Engine/Physics/ColliderComponent.h"
#include "Engine/Physics/RigidBodyComponent.h"
#include "Engine/Render/TextureManager.h"
#include "Engine/Render/Text.h"
#include "Engine/ECS/ECS.h"
#include "Engine/Common/DeltaTime.h"
#include "Engine/Physics/AABBTree.h"
#include "Engine/Physics/Manifold.h"
#include "Engine/Render/RenderUtils.h"

#include <iostream>

namespace Engine::Physics {
    class AABBTreeSDLVisualizer : public AABBTreeVisualizer {
    public:
        explicit AABBTreeSDLVisualizer(SDL_Renderer *renderer)
                : _renderer(renderer) {}

        void render(Math::Rect_d rect) override {
            auto sdlRect = (SDL_Rect)rect;
            SDL_SetRenderDrawColor(
                    _renderer,
                    0, 0, 255, 255);
            SDL_RenderDrawRect(
                    _renderer,
                    &sdlRect);
        }

    private:
        SDL_Renderer *_renderer;
    };

    class PhysicsSystem : public ECS::System {
    public:
        void update() override {

            _collidersToDraw.clear();

            auto dt = DeltaTime::get();

            auto bodies = ECS::EntityManager::getInstance()->
                    getEntitiesThatHaveComponent<RigidBodyComponent>();

            if (_tickCounter++ % _treeRebuildTicks == 0)
                rebuildTree(bodies);
            else
                updateTree(bodies);

            for (auto &body : bodies) {
                auto *rigid = body->getComponent<RigidBodyComponent>();
                rigid->integrateAcceleration(dt);
            }

            _collidersToDraw.resize(bodies.size());
            for (size_t i = 0; i < bodies.size(); ++i) {
                ECS::Entity *bodyA = bodies[i];
                Math::Rect_d boxA = bodyA->getComponent<ColliderComponent>()->getPositionedBB();
                auto targets = _tree.query(boxA);
                int realTargets = 0;
                for (auto it = targets.begin(); it != targets.end(); ++it) {
                    auto *pnt = *it;
//                for (auto *pnt : targets) {
                    auto bodyB = reinterpret_cast<ECS::Entity *>(pnt);
                    if (bodyA == bodyB)
                        continue;
                    Manifold manifold(bodyA, bodyB);
                    if (manifold.detect()) {
                        ++realTargets;
                        std::get<1>(_collidersToDraw[i]) = true;
//                        manifold.resolve();
                    }
                }
//                std::cout << targets.size() << "\n";
            }

            for (int iter = 0; iter < 5; ++iter) {
                for (ECS::Entity *bodyA : bodies) {
                    Math::Rect_d boxA = bodyA->getComponent<ColliderComponent>()->getPositionedBB();
                    auto targets = _tree.query(boxA);
                    for (auto *pnt : targets) {
//                        auto *pnt = targets[i];
                        auto bodyB = reinterpret_cast<ECS::Entity *>(pnt);
                        if (bodyA >= bodyB)
                            continue;
                        Manifold manifold(bodyA, bodyB);
                        if (manifold.detect()) {
                            manifold.resolve();
                            manifold.positionalCorrection();
                        }
                    }
                }
                updateTree(bodies);
            }

            for (auto &body : bodies) {
                auto *rigid = body->getComponent<RigidBodyComponent>();
                rigid->integrateVelocity(dt);
            }

            updateTree(bodies);

            for (size_t i = 0; i < bodies.size(); ++i) {
                auto &body = bodies[i];
                auto *rigid = body->getComponent<RigidBodyComponent>();
                auto *collider = body->getComponent<ColliderComponent>();

                rigid->acceleration = {0, 0};
                rigid->torque = 0;
                std::get<0>(_collidersToDraw[i]) = collider;
            }
        }

        void render() override {
            auto *renderer = Render::TextureManager::getInstance()->getDefaultRenderer();
            auto *sdlRenderer = renderer->get();

            AABBTreeSDLVisualizer viz(sdlRenderer);
            _tree.visualize(viz);

            for (auto[collider, color] : _collidersToDraw) {
                Math::Vector2f p1, p2, p3, p4;
                collider->getCorners(p1, p2, p3, p4);

                Render::drawCircle(renderer, {255, 0, 0, 255},
                                   collider->getOrigin(), 2);

                if (color)
                    Render::drawQuadrangle(renderer, {255, 0, 0, 255},
                                           p1, p2, p3, p4);
                else
                    Render::drawQuadrangle(renderer, {0, 255, 0, 255},
                                           p1, p2, p3, p4);
            }
        }

    private:
        AABBTree _tree;
        std::size_t _tickCounter = 0;
        const std::size_t _treeRebuildTicks = 60;
        std::vector<std::tuple<ColliderComponent *, bool>> _collidersToDraw;

        void rebuildTree(const std::vector<ECS::Entity *> &bodies) {
            _tree.clear();
            for (auto *body : bodies)
                _tree.insert(body, body->getComponent<ColliderComponent>()->getPositionedBB());
        }

        void updateTree(const std::vector<ECS::Entity *> &bodies) {
            for (auto *body : bodies)
                _tree.update(body, body->getComponent<ColliderComponent>()->getPositionedBB());
        }

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