#ifndef BACHERO_ENGINE_PHYSICS_SYSTEM
#define BACHERO_ENGINE_PHYSICS_SYSTEM

#include "Engine/Physics/ColliderComponent.h"
#include "Engine/Physics/RigidBodyComponent.h"
#include "Engine/Render/TextureManager.h"
#include "Engine/Render/Text.h"
#include "Engine/ECS/ECS.h"
#include "Engine/Common/DeltaTime.h"
#include "Engine/Common/DebugMode.h"
#include "Engine/Physics/AABBTree.h"
#include "Engine/Physics/Manifold.h"
#include "Engine/Render/RenderUtils.h"
#include "Engine/Common/HashMap.h"
#include "Engine/Render/CameraComponent.h"

#include <iostream>
#include <unordered_map>

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

            auto dt = DeltaTime::get();

            getBodies();
            applyLocks();

            if (_tickCounter++ % _treeRebuildTicks == 0)
                rebuildTree();
            else
                updateTree();

            broadPhase(dt);
            removeInvalidManifolds();
            integrateForces(dt);
            calcPreStep(dt);
            performIterativeCollisionResolution();
            integrateVelocities(dt);

            updateTree();
        }

        void render() override {
            if (DebugMode::isEnabled()) {
                if (auto *camera = Render::getActiveCamera()) {
                    auto *renderer = Render::TextureManager::getInstance()->getDefaultRenderer();

                    Math::Vector2f cameraPos =
                            camera->getComponent<TransformComponent>()->position;

//                AABBTreeSDLVisualizer viz(sdlRenderer);
//                _tree.visualize(viz);

                    for (auto *body : _bodies) {
                        if (!body->isActive())
                            continue;
                        auto *collider = body->getComponent<ColliderComponent>();

                        Math::Vector2f p1, p2, p3, p4;
                        collider->getCorners(p1, p2, p3, p4);

                        Render::drawCircle(renderer, {255, 0, 0, 255},
                                           collider->getAnchorPoint() - cameraPos, 2);

                        Render::drawQuadrangle(renderer, {0, 255, 0, 255},
                                               p1 - cameraPos, p2 - cameraPos,
                                               p3 - cameraPos, p4 - cameraPos);
                    }

                    for (auto&[key, manifold] : _manifolds)
                        for (std::size_t i = 0; i < manifold.contactsNum; ++i) {
                            auto &contact = manifold.contacts[i];
                            Render::drawCircle(renderer, {255, 0, 0, 255},
                                               contact.position - cameraPos, 2);
                        }
                }
            }
        }

    private:
        AABBTree _tree;
        std::size_t _tickCounter = 0;
        const std::size_t _treeRebuildTicks = 60;
        std::vector<ECS::Entity *> _bodies;
        HashMap<ManifoldKey, Manifold> _manifolds;

        void getBodies() {
            ECS::EntityManager::getInstance()->
                    getEntitiesThatHaveComponent<RigidBodyComponent>(_bodies, true);
        }

        void rebuildTree() {
            _tree.clear();
            for (auto *body : _bodies)
                if (body->isActive())
                    _tree.insert(body, body->getComponent<ColliderComponent>()->getPositionedBB());
        }

        void updateTree() {
            for (auto *body : _bodies)
                if (body->isActive())
                    _tree.update(body, body->getComponent<ColliderComponent>()->getPositionedBB());
                else
                    _tree.erase(body, body->getComponent<ColliderComponent>()->getPositionedBB());
        }

        void applyLocks() {
            for (auto &body : _bodies)
                if (body->isActive()) {
                    auto *rigid = body->getComponent<RigidBodyComponent>();
                    if (rigid->lockedRotation())
                        rigid->angularVelocity = 0.0f;
                }
        }

        void broadPhase(double dt) {
            for (auto *bodyA : _bodies) {
                if (!bodyA->isActive())
                    continue;
                auto *collider = bodyA->getComponent<ColliderComponent>();

                Math::Rect_d boxA = collider->getPositionedBB();
                auto targets = _tree.query(boxA);

                for (auto *pnt : targets) {
                    auto bodyB = reinterpret_cast<ECS::Entity *>(pnt);
                    if (bodyA == bodyB)
                        continue;

                    ManifoldKey key(bodyA, bodyB);
                    auto it = _manifolds.find(key);
                    Manifold manifold(bodyA, bodyB);
                    if (manifold.detectAndCalcCollision()) {
                        if (it == _manifolds.end())
                            _manifolds[key] = manifold;
                        else
                            it->second.detectAndCalcCollision();
                    } else {
                        _manifolds.erase(key);
                    }
                }
            }
        }

        void removeInvalidManifolds() {
            std::vector<ManifoldKey> manifoldsToRemove;
            for (auto&[key, manifold] : _manifolds)
                if (!manifold.bodyA->isActive() ||
                    !manifold.bodyB->isActive() ||
                    !manifold.detectAndCalcCollision())
                    manifoldsToRemove.push_back(key);
            for (auto &key : manifoldsToRemove)
                _manifolds.erase(key);
        }

        void calcPreStep(double dt) {
            for (auto&[key, manifold] : _manifolds)
                manifold.preStep(dt);
        }

        void performIterativeCollisionResolution() {
            for (int iter = 0; iter < 15; ++iter)
                for (auto&[key, manifold] : _manifolds)
                    manifold.resolve();
        }

        void integrateForces(double dt) {
            for (auto &body : _bodies)
                if (body->isActive()) {
                    auto *rigid = body->getComponent<RigidBodyComponent>();
                    rigid->integrateForces(dt);
                }
        }

        void integrateVelocities(double dt) {
            for (auto &body : _bodies)
                if (body->isActive()) {
                    auto *rigid = body->getComponent<RigidBodyComponent>();
                    rigid->integrateVelocity(dt);
                }
        }

        void clearForces() {
            for (auto &body : _bodies)
                if (body->isActive()) {
                    auto *rigid = body->getComponent<RigidBodyComponent>();

                    rigid->acceleration = {0, 0};
                    rigid->torque = 0;
                }
        }
    };
}

#endif //BACHERO_ENGINE_PHYSICS_SYSTEM