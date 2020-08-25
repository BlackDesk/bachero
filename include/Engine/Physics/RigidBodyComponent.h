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
                  restitution(_restitution), friction(_friction) {
        }

        void init() override {
            if (!owner->hasComponent<TransformComponent>())
                owner->addComponent<TransformComponent>();
            if (!owner->hasComponent<ColliderComponent>())
                throw std::runtime_error("RigidBody requires Collider.");

            _transform = owner->getComponent<TransformComponent>();
            _collider = owner->getComponent<ColliderComponent>();
            Math::Rect_f bb = _collider->getBoundingBox();

            const_cast<float &>(I) = mass * (
                    bb.size.lengthSqr() / 12.0f +
                    _collider->getCenterOfMass().lengthSqr()
            );

            const_cast<float &>(invI) = (I > 0.0f ? 1.0f / I : 0.0f);
        }

        void applyVelocity(Math::Vector2d vel) {
            velocity += vel;
        }

        //radius is a vector from anchor point to point of where impulse applied
        void applyImpulse(Math::Vector2f impulse, Math::Vector2f radius) {
            velocity += invMass * impulse;
            angularVelocity += invI * radius.pseudoDotProduct(impulse);
        }

        void applyForce(Math::Vector2d force) {
            acceleration += force * invMass;
        }

        void applyAcceleration(Math::Vector2d accel) {
            acceleration += accel;
        }

        void applyTorque(float M) {
            torque += M;
        }

        void integrateVelocity(double dt) {
            if (invMass == 0.0)
                return;

            _transform->position += velocity * dt;

            if (!lockedRotation)
                _transform->rotation += angularVelocity * dt;
        }

        void integrateForces(double dt) {
            if (invMass == 0.0)
                return;

            velocity += acceleration * dt;
            angularVelocity += torque * dt;
        }

        bool isStatic() const {
            return mass == 0.0f;
        };

        Math::Vector2f acceleration;
        Math::Vector2f velocity;

        const float mass;
        const float invMass;
        const float I = 0.0f, invI = 0.0f;
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