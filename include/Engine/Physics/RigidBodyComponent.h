#ifndef BACHERO_ENGINE_PHYSICS_RIGIDBODY_COMPONENT
#define BACHERO_ENGINE_PHYSICS_RIGIDBODY_COMPONENT

#include "Engine/Physics/ColliderComponent.h"

namespace Engine::Physics {
    class RigidBodyComponent : public ECS::DataOnlyComponent {
    public:
        explicit RigidBodyComponent(float _mass = 0,
                                    float _restitution = 0,
                                    float _friction = 0)
                : mass(_mass), invMass(_mass > 0.0f ? 1.0f / _mass : 0.0f),
                  restitution(_restitution), friction(_friction) {}

        void init() override {
            if (!owner->hasComponent<TransformComponent>())
                owner->addComponent<TransformComponent>();
            if (!owner->hasComponent<ColliderComponent>())
                throw std::runtime_error("RigidBody requires Collider.");
            _transform = owner->getComponent<TransformComponent>();
            _collider = owner->getComponent<ColliderComponent>();
        }

        void applyVelocity(Math::Vector2d vel) {
            velocity += vel;
        }

        void applyImpulse(Math::Vector2d impulse) {
            velocity += invMass * impulse;
        }

        void applyForce(Math::Vector2d force) {
            acceleration += force * invMass;
        }

        void applyAcceleration(Math::Vector2d accel) {
            acceleration += accel;
        }

        void integrateVelocity(double dt) {
            if (invMass == 0.0)
                return;

            _transform->position += velocity * dt;

            if (!lockedRotation)
                _transform->rotation += angularVelocity * dt;
        }

        void integrateAcceleration(double dt) {
            if (invMass == 0.0)
                return;

            velocity += acceleration * dt;
        }

        bool isStatic() const {
            return mass == 0.0f;
        };

        Math::Vector2f acceleration;
        Math::Vector2f velocity;

        const float mass;
        const float invMass;
        const float restitution;
        const float friction;

        float torque = 0;
        float angularVelocity = 0;

        bool lockedRotation = false;

    private:
        TransformComponent *_transform = nullptr;
        ColliderComponent *_collider = nullptr;
    };
}

#endif //BACHERO_ENGINE_PHYSICS_RIGIDBODY_COMPONENT